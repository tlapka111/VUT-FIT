-- cpu.vhd: Simple 8-bit CPU (BrainF*ck interpreter)
-- Copyright (C) 2018 Brno University of Technology,
--                    Faculty of Information Technology
-- Author(s): xtlapa00
--

library ieee;
use ieee.std_logic_1164.all;
use ieee.std_logic_arith.all;
use ieee.std_logic_unsigned.all;

-- ----------------------------------------------------------------------------
--                        Entity declaration
-- ----------------------------------------------------------------------------
entity cpu is
 port (
   CLK   : in std_logic;  -- hodinovy signal
   RESET : in std_logic;  -- asynchronni reset procesoru
   EN    : in std_logic;  -- povoleni cinnosti procesoru
 
   -- synchronni pamet ROM
   CODE_ADDR : out std_logic_vector(11 downto 0); -- adresa do pameti
   CODE_DATA : in std_logic_vector(7 downto 0);   -- CODE_DATA <- rom[CODE_ADDR] pokud CODE_EN='1'
   CODE_EN   : out std_logic;                     -- povoleni cinnosti
   
   -- synchronni pamet RAM
   DATA_ADDR  : out std_logic_vector(9 downto 0); -- adresa do pameti
   DATA_WDATA : out std_logic_vector(7 downto 0); -- mem[DATA_ADDR] <- DATA_WDATA pokud DATA_EN='1'
   DATA_RDATA : in std_logic_vector(7 downto 0);  -- DATA_RDATA <- ram[DATA_ADDR] pokud DATA_EN='1'
   DATA_RDWR  : out std_logic;                    -- cteni z pameti (DATA_RDWR='1') / zapis do pameti (DATA_RDWR='0')
   DATA_EN    : out std_logic;                    -- povoleni cinnosti
   
   -- vstupni port
   IN_DATA   : in std_logic_vector(7 downto 0);   -- IN_DATA obsahuje stisknuty znak klavesnice pokud IN_VLD='1' a IN_REQ='1'
   IN_VLD    : in std_logic;                      -- data platna pokud IN_VLD='1'
   IN_REQ    : out std_logic;                     -- pozadavek na vstup dat z klavesnice
   
   -- vystupni port
   OUT_DATA : out  std_logic_vector(7 downto 0);  -- zapisovana data
   OUT_BUSY : in std_logic;                       -- pokud OUT_BUSY='1', LCD je zaneprazdnen, nelze zapisovat,  OUT_WE musi byt '0'
   OUT_WE   : out std_logic                       -- LCD <- OUT_DATA pokud OUT_WE='1' a OUT_BUSY='0'
 );
end cpu;


-- ----------------------------------------------------------------------------
--                      Architecture declaration
-- ----------------------------------------------------------------------------
architecture behavioral of cpu is

	signal PC : std_logic_vector (11 downto 0);
	signal PC_dec : std_logic;
	signal PC_inc : std_logic;
 
	signal CNT : std_logic_vector (7 downto 0);
	signal CNT_dec : std_logic;
	signal CNT_inc : std_logic;
 
	signal PTR : std_logic_vector (9 downto 0);
	signal PTR_dec : std_logic;
	signal PTR_inc : std_logic;

	signal MX : std_logic_vector (1 downto 0);
	signal part_of_instruction : std_logic_vector (7 downto 0);

	type fsm is 
	(
		IDLE, FETCH, DECODE, 
		INC_PTR, DEC_PTR, 
		CELL_INC, CELL_INC_1 , CELL_DEC, CELL_DEC_1,
		WHILE_START, WHILE_START_1, WHILE_START_2, WHILE_START_3,
		WHILE_END, WHILE_END_1, WHILE_END_2, WHILE_END_3, WHILE_END_4, WHILE_END_5,  
		PRINT, PRINT_1, 
		LOAD_TO_CELL,
		COMMENTARY, COMMENTARY_1, COMMENTARY_2,
		CHARS_NUMS,
		TERMINATOR,
		S_OTHERS
	);
	
	signal state: fsm;
	signal nstate: fsm;

begin

	-- PC registr
 
	programovy_citac: process (CLK, RESET, PC, PC_dec, PC_inc)
 
	begin
		if (RESET = '1') then
			PC <= (others => '0');
		elsif (CLK'event and CLK = '1') then
			if (PC_dec = '1') then
				PC <= PC - 1 ;
			elsif (PC_inc = '1') then
				PC <= PC + 1;
			end if;
		end if;
		
		CODE_ADDR <= PC;
		
	end process;

	-- CNT registr
	
	bracket_counter: process (CLK, RESET, CNT, CNT_dec, CNT_inc)

	begin
		if (RESET = '1') then
			CNT <= (others => '0');
		elsif (CLK'event and CLK = '1') then
			if (CNT_dec = '1') then
				CNT <= CNT - 1 ;
			elsif (CNT_inc = '1') then
				CNT <= CNT + 1;
			end if;
		end if;
	end process;
	
	-- PTR registr
	
	data_pointer: process (CLK, RESET, PTR, PTR_dec, PTR_inc)
	
	begin
		if (RESET = '1') then
			PTR <= (others => '0');
		elsif (CLK'event and CLK = '1') then
			if (PTR_dec = '1') then
				PTR <= PTR - 1 ;
			elsif (PTR_inc = '1') then
				PTR <= PTR + 1;
			end if;
		end if;
		
		DATA_ADDR <= PTR;
		
	end process;
	
	-- multliplexor
	
	multiplexor: process (MX, DATA_RDATA, IN_DATA, part_of_instruction)
	begin
		case (MX) is
			when "00" => DATA_WDATA <= IN_DATA; -- hodnoza ze vstupu
			when "01" => DATA_WDATA <= DATA_RDATA + 1; -- hodnota *ptr += 1
			when "10" => DATA_WDATA <= DATA_RDATA - 1; -- hodnota *ptr -= 1
			when "11" => DATA_WDATA <= part_of_instruction; -- hodnota souèástí instrukce
			when others =>
		end case;
		
	end process;
	
	-- FSM
	
	actual_state: process (CLK, RESET, EN)
	begin
		if (RESET= '1') then
			state <= IDLE;
		elsif (CLK'event and CLK = '1') then
			if (EN = '1') then
				state <= nstate;
			end if;
		end if;
	end process;
	
	next_state: process (CODE_DATA, DATA_RDATA, IN_VLD, OUT_BUSY, state, CNT)
	begin
		CODE_EN <= '1';
		DATA_EN <= '0';
		DATA_RDWR <= '0';
		OUT_WE <= '0';
		IN_REQ <= '0';
		PC_dec <= '0';
		PC_inc <= '0';
		CNT_dec <= '0';
		CNT_inc <= '0';
		PTR_dec <= '0';
		PTR_inc <= '0';
		MX <= "00";
		
		case state is
			when IDLE =>
				nstate <= FETCH;
			
			when FETCH =>
				CODE_EN <= '1';
				nstate <= DECODE;
				
			when DECODE =>
				case (CODE_DATA) is
					when X"3E" => nstate <= INC_PTR; -- inkrementace hodnoty ukazatele
               when X"3C" => nstate <= DEC_PTR; -- dekrementace hodnoty ukazatele
               when X"2B" => nstate <= CELL_INC; -- inkrementace hodnoty aktualni bunky
               when X"2D" => nstate <= CELL_DEC; -- dekrementace hodnoty aktualni bunky 
               when X"5B" => nstate <= WHILE_START; -- je-li hodnota aktualni bunky nulova, skoc za odpo
															  -- vidajici prikaz, jinak pokracuj nasledujicim znakem
					when X"5D" => nstate <= WHILE_END; -- je-li hodnota aktualni bunky nenulova, skoc za odpovi
															  -- dajici prikaz [ jinak pokracuj nasledujicim znakem
               when X"2E" => nstate <= PRINT; -- vytiskni hodnotu aktualni bunky
               when X"2C" => nstate <= LOAD_TO_CELL; -- nacti hodnotu a uloz ji do aktualni bunky
               when X"23" => nstate <= COMMENTARY; -- vsechny znaky mezi timto a nasledujicim znakem # /* ... */
															  -- jsou ignorovany (blokovy komentar)
               when X"30"|X"31"|X"32"|X"33"|X"34"|X"35"|X"36"|X"37"|X"38"|X"39"|X"41"|X"42"|X"43"|X"44"|X"45"|X"46" => nstate <= CHARS_NUMS;              
					when X"00" => nstate <= TERMINATOR; -- zastav vykonavani programu
					when others => nstate <= S_OTHERS; 
				end case;

			WHEN INC_PTR =>
				PTR_inc <= '1';
				PC_inc <= '1';
				nstate <= FETCH;
				
			WHEN DEC_PTR =>
				PTR_dec <= '1';
				PC_inc <= '1';
				nstate <= FETCH;
				
			WHEN CELL_INC =>
				DATA_EN <= '1';
				DATA_RDWR <= '1';
				nstate <= CELL_INC_1;
			
			WHEN CELL_INC_1 =>
				DATA_EN <= '1';
				DATA_RDWR <= '0';
				MX <= "01";				
				PC_inc <= '1';
				nstate <= FETCH;
			
			WHEN CELL_DEC =>
				DATA_EN <= '1';
				DATA_RDWR <= '1';
				nstate <= CELL_DEC_1;
				
			WHEN CELL_DEC_1 =>
				DATA_EN <= '1';
				DATA_RDWR <= '0';
				MX <= "10";
				PC_inc <= '1';
				nstate <= FETCH;
				
				
			WHEN WHILE_START =>
				PC_inc <= '1';
				DATA_EN <= '1';
            DATA_RDWR <= '1';
				nstate <= WHILE_START_1;
				
			WHEN WHILE_START_1 =>
				if(DATA_RDATA = "00000000") then
					CNT_inc <= '1';
					nstate <= WHILE_START_2;
				else
					nstate <= FETCH;
				end if;
				
			WHEN WHILE_START_2 =>
				if(CNT = "00000000") then
					nstate <= FETCH;
            else
					CODE_EN <= '1';
					nstate <= WHILE_START_3;
				end if;
			
			WHEN WHILE_START_3 =>
			if (CODE_DATA) = X"5B" then
				CNT_inc <= '1';
         elsif CODE_DATA = X"5D" then
				CNT_dec <= '1';
         end if;
         PC_inc <= '1';
         nstate <= WHILE_START_2;
			
			WHEN WHILE_END =>
				 DATA_EN <= '1';
             DATA_RDWR <= '1';
				 nstate <= WHILE_END_1;
				
			WHEN WHILE_END_1 =>
				 if(DATA_RDATA = "00000000") then
					PC_inc <= '1';
					nstate <= FETCH;
				 else
					nstate <= WHILE_END_2;
				 end if;
				 
			WHEN WHILE_END_2 =>
				CNT_inc <= '1';
				PC_dec <= '1';
				nstate <= WHILE_END_3;
				
			WHEN WHILE_END_3 =>
				if (CNT = "00000000") then
					nstate <= FETCH;
				else
					CODE_EN <= '1';
					nstate <= WHILE_END_4;
				end if;
				
			WHEN WHILE_END_4 =>
				if(CODE_DATA = X"5D") then
					CNT_inc <= '1';
            elsif(CODE_DATA = X"5B") then
               CNT_dec <= '1';
            end if;
            nstate <= WHILE_END_5;
			
			WHEN WHILE_END_5 =>
				if(CNT = "00000000") then
					PC_inc <= '1';
            else
               PC_dec <= '1';
            end if;
            nstate <= WHILE_END_3;
			
			WHEN PRINT =>
				if (OUT_BUSY = '1') then
					nstate <= PRINT;
				else	
					DATA_EN <= '1';
					DATA_RDWR <= '1';
					nstate <= PRINT_1;
				end if;
				
			WHEN PRINT_1 =>
				OUT_WE <= '1';
				OUT_DATA <= DATA_RDATA;
				PC_inc <= '1';
				nstate <= FETCH;
			
			WHEN LOAD_TO_CELL =>
				IN_REQ <= '1';
				if (IN_VLD = '0') then
					nstate <= LOAD_TO_CELL;
				else
					MX <= "00";
					DATA_EN <= '1';
					DATA_RDWR <= '0';
					PC_inc <= '1';
					nstate <= FETCH;
				end if;
			
			WHEN COMMENTARY =>
				PC_inc <= '1';
				nstate <= COMMENTARY_1;
				
			WHEN COMMENTARY_1 =>
				CODE_EN <= '1';
				nstate <= COMMENTARY_2;
			
			WHEN COMMENTARY_2 =>
				if (CODE_DATA /= X"23") then
					nstate <= COMMENTARY;
				else
					PC_inc <= '1';
					nstate <= FETCH;
				end if;	
			
			WHEN CHARS_NUMS =>
				DATA_EN <= '1';
				PC_inc <= '1';
				MX <= "11";
				if (CODE_DATA = X"31") then
					part_of_instruction <= "00010000"; --16
				elsif (CODE_DATA = X"32") then
					part_of_instruction <= "00100000"; -- 32
				elsif (CODE_DATA = X"33") then
					part_of_instruction <= "00110000"; -- 48
				elsif (CODE_DATA = X"34") then
					part_of_instruction <= "01000000"; -- 64
				elsif (CODE_DATA = X"35") then
					part_of_instruction <= "01010000"; -- 80
				elsif (CODE_DATA = X"36") then
					part_of_instruction <= "01100000"; -- 96
				elsif (CODE_DATA = X"37") then
					part_of_instruction <= "01110000"; -- 112
				elsif (CODE_DATA = X"38") then
					part_of_instruction <= "10000000"; -- 128
				elsif (CODE_DATA = X"39") then
					part_of_instruction <= "10010000"; -- 144
				elsif (CODE_DATA = X"30") then
					part_of_instruction <= "00000000"; -- 0
				elsif(CODE_DATA = X"41") then
					part_of_instruction <= "10100000"; -- 160
				elsif (CODE_DATA = X"42") then
					part_of_instruction <= "10110000"; -- 176
				elsif (CODE_DATA = X"43") then
					part_of_instruction <= "11000000"; -- 192
				elsif (CODE_DATA = X"44") then
					part_of_instruction <= "11010000"; -- 208
				elsif (CODE_DATA = X"45") then
					part_of_instruction <= "11100000"; -- 224
				elsif (CODE_DATA = X"46") then
					part_of_instruction <= "11110000"; -- 240	
				end if;
				nstate <= FETCH;
		
			when TERMINATOR =>
				nstate <= TERMINATOR;
			
			when S_OTHERS =>
				nstate <= FETCH;
				PC_inc <= '1';
				
			when others => null;
		end case;	

	end process;
end behavioral;
 
	 
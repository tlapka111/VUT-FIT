-- fsm.vhd: Finite State Machine
-- Author(s): 
--
library ieee;
use ieee.std_logic_1164.all;
-- ----------------------------------------------------------------------------
--                        Entity declaration
-- ----------------------------------------------------------------------------
entity fsm is
port(
   CLK         : in  std_logic;
   RESET       : in  std_logic;

   -- Input signals
   KEY         : in  std_logic_vector(15 downto 0);
   CNT_OF      : in  std_logic;

   -- Output signals
   FSM_CNT_CE  : out std_logic;
   FSM_MX_MEM  : out std_logic;
   FSM_MX_LCD  : out std_logic;
   FSM_LCD_WR  : out std_logic;
   FSM_LCD_CLR : out std_logic
);
end entity fsm;

-- ----------------------------------------------------------------------------
--                      Architecture declaration
-- ----------------------------------------------------------------------------
-- PRINT_MESSAGE = ERROR, PRINT_MESSAGE1 = CORRECT
architecture behavioral of fsm is
   type t_state is (START, DIGIT_1, DIGIT_2, DIGIT_3, DIGIT_4, DIGIT_5, DIGIT_6_PSW_1, DIGIT_6_PSW_2, DIGIT_7_PSW_1, DIGIT_7_PSW_2, DIGIT_8_PSW_1, DIGIT_8_PSW_2, DIGIT_9_PSW_1, DIGIT_9_PSW_2, DIGIT_10_PSW_1, DIGIT_10_PSW_2, WRONG_NUMBER, PRINT_MESSAGE, PRINT_MESSAGE1, FINISH);
   signal present_state, next_state : t_state;

begin
-- -------------------------------------------------------
sync_logic : process(RESET, CLK)
begin
   if (RESET = '1') then
      present_state <= START;
   elsif (CLK'event AND CLK = '1') then
      present_state <= next_state;
   end if;
end process sync_logic;

-- password1: 13799 6 8 697 password2: 13799 9 8 552
-- -------------------------------------------------------
next_state_logic : process(present_state, KEY, CNT_OF)
begin
   case (present_state) is
   -- - - - - - - - - - - - - - - - - - - - - - -
   
	when START =>
      next_state <= START;
		
		if (KEY(1) = '1') then
			next_state <= DIGIT_1;
		
      elsif (KEY(15) = '1') then
         next_state <= PRINT_MESSAGE; 
      
		elsif (KEY(15 downto 0) /= "0000000000000000") then
	      next_state <= WRONG_NUMBER;
		end if;
		
   -- - - - - - - - - - - - - - - - - - - - - - -
  
  when DIGIT_1 =>
		next_state <= DIGIT_1;
		
		if (KEY(3) = '1') then
			next_state <= DIGIT_2;
		
		elsif (KEY(15) = '1') then
			next_state <= PRINT_MESSAGE;
		
		elsif (KEY(15 downto 0) /= "0000000000000000") then
	      next_state <= WRONG_NUMBER;
		end if;
		
		-- - - - - - - - - - - - - - - - - - - - - - -
	
	when DIGIT_2 =>
		next_state <= DIGIT_2;
		
		if (KEY(7) = '1') then
			next_state <= DIGIT_3;
		
		elsif (KEY(15) = '1') then
			next_state <= PRINT_MESSAGE;
		
		elsif (KEY(15 downto 0) /= "0000000000000000") then
	      next_state <= WRONG_NUMBER;
		end if;
	
	-- - - - - - - - - - - - - - - - - - - - - - - - 
	
	when DIGIT_3 =>
		next_state <= DIGIT_3;
		
		if (KEY(9) = '1') then
			next_state <= DIGIT_4;
		
		elsif (KEY(15) = '1') then
			next_state <= PRINT_MESSAGE;
		
		elsif (KEY(15 downto 0) /= "0000000000000000") then
	      next_state <= WRONG_NUMBER;
		end if;
	
	-- - - - - - - - - - - - - - - - - - - - - - - -
	
	when DIGIT_4 =>
		next_state <= DIGIT_4;
		
		if (KEY(9) = '1') then
			next_state <= DIGIT_5;
		
		elsif (KEY(15) = '1') then
			next_state <= PRINT_MESSAGE;
		
		elsif (KEY(15 downto 0) /= "0000000000000000") then
	      next_state <= WRONG_NUMBER;
		end if;
	
	-- - - - - - - - - - - - - - - - - - - - - - - -
	
	when DIGIT_5 =>
		next_state <= DIGIT_5;
		
		if (KEY(6) = '1') then
			next_state <= DIGIT_6_PSW_1;
		
		elsif (KEY(9) = '1') then
			next_state <= DIGIT_6_PSW_2;
		
		elsif (KEY(15) = '1') then
			next_state <= PRINT_MESSAGE;
		
		elsif (KEY(15 downto 0) /= "0000000000000000") then
	      next_state <= WRONG_NUMBER;
		end if;
		
		-- - - - - - - - - - - - - - - - - - - - - - -
	
	when DIGIT_6_PSW_1 =>
		next_state <= DIGIT_6_PSW_1;
		
		if (KEY(8) = '1') then
			next_state <= DIGIT_7_PSW_1;
		
		elsif (KEY(15) = '1') then
			next_state <= PRINT_MESSAGE;
		
		elsif (KEY(15 downto 0) /= "0000000000000000") then
	      next_state <= WRONG_NUMBER;
		end if;
	
		-- - - - - - - - - - - - - - - - - - - - - - - -
	
	when DIGIT_7_PSW_1 =>
		next_state <= DIGIT_7_PSW_1;
		
		if (KEY(6) = '1') then
			next_state <= DIGIT_8_PSW_1;
		
		elsif (KEY(15) = '1') then
			next_state <= PRINT_MESSAGE;
		
		elsif (KEY(15 downto 0) /= "0000000000000000") then
	      next_state <= WRONG_NUMBER;
		end if;
		
		-- - - - - - - - - - - - - - - - - - - - - - - -
	
	when DIGIT_8_PSW_1 =>
		next_state <= DIGIT_8_PSW_1;
		
		if (KEY(9) = '1') then
			next_state <= DIGIT_9_PSW_1;
		
		elsif (KEY(15) = '1') then
			next_state <= PRINT_MESSAGE;
		
		elsif (KEY(15 downto 0) /= "0000000000000000") then
	      next_state <= WRONG_NUMBER;
		end if;
		
		-- - - - - - - - - - - - - - - - - - - - - - - - -
	
	when DIGIT_9_PSW_1 =>
		next_state <= DIGIT_9_PSW_1;
		
		if (KEY(7) = '1') then
			next_state <= DIGIT_10_PSW_1;
		
		elsif (KEY(15) = '1') then
			next_state <= PRINT_MESSAGE;
		
		elsif (KEY(15 downto 0) /= "0000000000000000") then
	      next_state <= WRONG_NUMBER;
		end if;
	
	-- - - - - - - - - - - - - - - - - - - - - - - - - - -
	
	when DIGIT_10_PSW_1 =>
		next_state <= DIGIT_10_PSW_1;
		
		if (KEY(15) = '1') then
			next_state <= PRINT_MESSAGE1;
			
		elsif (KEY(15 downto 0) /= "0000000000000000") then
	      next_state <= WRONG_NUMBER;
		end if;
	
	-- - - - - - - - - - - - - - - - - - - - - - - - -
	
	when DIGIT_6_PSW_2 =>
		next_state <= DIGIT_6_PSW_2; 
	
		if (KEY(8) = '1') then
			next_state <= DIGIT_7_PSW_2;
		
		elsif (KEY(15) = '1') then
			next_state <= PRINT_MESSAGE;
		
		elsif (KEY(15 downto 0) /= "0000000000000000") then
	      next_state <= WRONG_NUMBER;
		end if;
		
		-- - - - - - - - - - - - - - - - - - - - - - - - 
	
	when DIGIT_7_PSW_2 =>
		next_state <= DIGIT_7_PSW_2;
		
		if (KEY(5) = '1') then
			next_state <= DIGIT_8_PSW_2;
		
		elsif (KEY(15) = '1') then
			next_state <= PRINT_MESSAGE;
		
		elsif (KEY(15 downto 0) /= "0000000000000000") then
	      next_state <= WRONG_NUMBER;
		end if;
		
		-- - - - - - - - - - - - - - - - - - - - - - - 
	
	when DIGIT_8_PSW_2 =>
		next_state <= DIGIT_8_PSW_2;
		
		if (KEY(5) = '1') then
			next_state <= DIGIT_9_PSW_2;
		
		elsif (KEY(15) = '1') then
			next_state <= PRINT_MESSAGE;
		
		elsif (KEY(15 downto 0) /= "0000000000000000") then
	      next_state <= WRONG_NUMBER;
		end if;
		
		-- - - - - - - - - - - - - - - - - - - - - - - - -
	
	when DIGIT_9_PSW_2 =>
		next_state <= DIGIT_9_PSW_2;
		
		if (KEY(2) = '1') then
			next_state <= DIGIT_10_PSW_2;
		
		elsif (KEY(15) = '1') then
			next_state <= PRINT_MESSAGE;
		
		elsif (KEY(15 downto 0) /= "0000000000000000") then
	      next_state <= WRONG_NUMBER;
		end if;
	
	-- - - - - - - - - - - - - - - - - - - - - - - - - - -
	
	when DIGIT_10_PSW_2 =>
		next_state <= DIGIT_10_PSW_2;
		
		if (KEY(15) = '1') then
			next_state <= PRINT_MESSAGE1;
			
		elsif (KEY(15 downto 0) /= "0000000000000000") then
	      next_state <= WRONG_NUMBER;
		end if;
	
	-- - - - - - - - - - - - - - - - - - - - - - - - -
	
	-- - - - - - - - - - - - - - - - - - - - - - - - - 
	
	when WRONG_NUMBER =>
		next_state <= WRONG_NUMBER;
		
		if (KEY(15) = '1') then
         next_state <= PRINT_MESSAGE;
      end if;
		
		-- - - - - - - - -- - - - - - - - -- - - - - -
	
	when PRINT_MESSAGE1 =>
      next_state <= PRINT_MESSAGE1;
      
		if (CNT_OF = '1') then
         next_state <= FINISH;
      end if;
		
	-- - - - - - - - - - - - - - - - - - - - - - -	
	
	when PRINT_MESSAGE =>
      next_state <= PRINT_MESSAGE;
      if (CNT_OF = '1') then
         next_state <= FINISH;
      end if;
		
   -- - - - - - - - - - - - - - - - - - - - - - -
   
	when FINISH =>
      next_state <= FINISH;
      if (KEY(15) = '1') then
         next_state <= START; 
      end if;
		
   -- - - - - - - - - - - - - - - - - - - - - - -
   
	when others =>
      next_state <= START;
   end case;
end process next_state_logic;

-- -------------------------------------------------------

output_logic : process(present_state, KEY)
begin
   FSM_CNT_CE     <= '0';
   FSM_MX_MEM     <= '0';
   FSM_MX_LCD     <= '0';
   FSM_LCD_WR     <= '0';
   FSM_LCD_CLR    <= '0';

   case (present_state) is
	
   -- - - - - - - - - - - - - - - - - - - - - - -
   
	when START =>
      if (KEY(14 downto 0) /= "000000000000000") then
         FSM_LCD_WR     <= '1';
      end if;
		
      if (KEY(15) = '1') then
         FSM_LCD_CLR    <= '1';
      end if;
	
	-- - - - - - - - - - - - - - - - - - - - - - -
	
	when PRINT_MESSAGE =>
      FSM_CNT_CE     <= '1';
      FSM_MX_LCD     <= '1';
      FSM_LCD_WR     <= '1';
		
   -- - - - - - - - - - - - - - - - - - - - - - -
   
	-- FSM_MX_MEM     <= '1'; nastavi zpravu na P�istup povolen.
	when PRINT_MESSAGE1 =>
      FSM_CNT_CE     <= '1';
      FSM_MX_LCD     <= '1';
      FSM_LCD_WR     <= '1';
		FSM_MX_MEM     <= '1';
		
		 -- - - - - - - - - - - - - - - - - - - - - - -
   
	when FINISH =>
      if (KEY(15) = '1') then
         FSM_LCD_CLR    <= '1';
      end if;
		
   -- - - - - - - - - - - - - - - - - - - - - - - -
   -- state OTHERS umo�nuje tisk v�ce kl�ves na display, bez OTHERS by musel m�t ka�d�
	-- stav pro svoji kl�vesu i sv�j output, kter� by umo�noval tisk vic� kl�ves na display
	when others =>
		if (KEY(14 downto 0) /= "000000000000000") then
         FSM_LCD_WR     <= '1';
      end if;
		
      if (KEY(15) = '1') then
         FSM_LCD_CLR    <= '1';
      end if;
		
	-- - - - - - - - - - - - - - - - - - - - - - - - - 
   end case;
end process output_logic;

end architecture behavioral;


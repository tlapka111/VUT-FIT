library IEEE;
use IEEE.std_logic_1164.all;
use IEEE.std_logic_arith.all;
use IEEE.std_logic_unsigned.all;

entity ledc8x8 is
port ( 

SMCLK : in std_logic;
RESET : in std_logic;
ROW 	: out std_logic_vector (0 to 7);
LED	: out std_logic_vector (0 to 7)

);
end ledc8x8;

architecture main of ledc8x8 is
 
signal clock_counter: std_logic_vector (11 downto 0) := (others => '0'); -- 7 372 800 / 256 / 8 -->> 3600 -->> bin 111000010000 -->> 12bitù
signal state: std_logic_vector (20 downto 0) := (others => '0'); -- 4 stavy (znak, nic, znak, nic), 7 372 800 / 4 -->> 1843200 -->> bin 111000010000000000000 -->> 21bitù
signal state_switch: std_logic_vector (1 downto 0) := (others => '0'); -- 4 stavy -->> 2 potøebné bity
signal row_n: std_logic_vector (7 downto 0) := (others => '0');
signal row_switch: std_logic := '0'; -- pøepínaní øádkù
signal led_n: std_logic_vector (7 downto 0) := (others => '0');

begin

	 
frequency_generator:process(SMCLK, RESET)
    begin
		if RESET = '1' then
			clock_counter <= (others => '0');
			
      elsif rising_edge(SMCLK) then
            clock_counter <= clock_counter + 1;
				if clock_counter = "111000010000" then 
					row_switch <= '1';
					clock_counter <=(others => '0');
				else
					row_switch <= '0';  -- reset pøepínaèe øádkù
				end if;				
      end if;
			
    end process frequency_generator;

state_generator:process(SMCLK,RESET)
	begin
		if RESET = '1' then
			state <=(others => '0');
		
		elsif rising_edge(SMCLK) then	
			state <= state +1;
			if state = "111000010000000000000" then
				state_switch <= state_switch +1;
				state <=(others => '0');
			end if;
		end if;	
	end process state_generator;
  
rows_rotation:process(SMCLK, RESET, row_switch)
	begin
		if RESET = '1' then
			row_n <= "10000000"; 
		elsif rising_edge(SMCLK) and row_switch = '1' then --podmínka pro pøeskoèení na další øádek
			row_n <= row_n(0) & row_n(7 downto 1); --  rotace bitù doprava
		end if;
	
	end process rows_rotation;
	
	
draw_leds:process(row_n)
	begin
		if state_switch = "00" then
			case row_n is	
				when "10000000" => led_n <= "00000111";
				when "01000000" => led_n <= "11011111";
				when "00100000" => led_n <= "11011111";
				when "00010000" => led_n <= "11011111";
				when "00001000" => led_n <= "11011111";
				when "00000100" => led_n <= "11111111";
				when "00000010" => led_n <= "11111111";
				when "00000001" => led_n <= "11111111";
				when others =>     led_n <= "11111111";
		end case;
		
		elsif state_switch = "01" or state_switch = "11" then
			case row_n is	
				when "10000000" => led_n <= "11111111";
				when "01000000" => led_n <= "11111111";
				when "00100000" => led_n <= "11111111";
				when "00010000" => led_n <= "11111111";
				when "00001000" => led_n <= "11111111";
				when "00000100" => led_n <= "11111111";
				when "00000010" => led_n <= "11111111";
				when "00000001" => led_n <= "11111111";
				when others =>     led_n <= "11111111";
		end case;
		
		elsif state_switch = "10" then 
			case row_n is	
				when "10000000" => led_n <= "11111111";
				when "01000000" => led_n <= "11111111";
				when "00100000" => led_n <= "11111111";
				when "00010000" => led_n <= "11111111";
				when "00001000" => led_n <= "11100000";
				when "00000100" => led_n <= "11111011";
				when "00000010" => led_n <= "11111011";
				when "00000001" => led_n <= "11111011";
				when others =>     led_n <= "11111011";
			end case;
		end if;
		
	end process draw_leds;

ROW <= row_n;
LED <= led_n;

end main;

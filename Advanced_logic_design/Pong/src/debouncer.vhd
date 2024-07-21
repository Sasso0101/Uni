library ieee;
use ieee.std_logic_1164.all;
use ieee.numeric_std.all;
use work.utilities.all;

entity debouncer is
    generic (
        debouncer_counter_size : integer := 12
    );
    port (
        clk   : in std_logic;
        res : in std_logic;
        bouncy : in std_logic;
        stable_value : out std_logic;
        pulse : out std_logic
    );
end entity;

architecture rtl of debouncer is
    -- Counter that decreases at every clock if the signal is stable
    signal counter: unsigned (debouncer_counter_size - 1 downto 0) := (others => '0') ;
    signal hold_counter: unsigned (HOLD_COUNTER_SIZE - 1 downto 0);
    -- Counter that decreases at every clock if the signal is stable
    signal candidate_value: std_logic;
begin
    process (clk, res)
    begin
        -- If signal is stable for a long time, then set stable_value to candidate_value
        if res = '0' then
            counter <= ( others => '1' );
            candidate_value <= '0';
            stable_value <= '0';
            hold_counter <= (others => '0'); 
            pulse <= '0';
        elsif rising_edge(clk) then
            if bouncy = candidate_value then
                if counter = 0 then
                    stable_value <= candidate_value;
                    if candidate_value then
                        pulse <= '1' when hold_counter = 0 else '0';
                        hold_counter <= hold_counter - 1;
                    end if;
                else
                    counter <= counter - 1;
                end if;
            else
                candidate_value <= bouncy;
                counter <= (others => '1');
                hold_counter <= (others => '0'); 
            end if;
        end if;
    end process;
end architecture;
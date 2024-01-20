library ieee;
use ieee.std_logic_1164.all;
use ieee.numeric_std.all;

entity debouncer is
    generic (
        counter_size : integer := 3
    );
    port (
        clk   : in std_logic;
        reset : in std_logic;
        bouncy : in std_logic;
        pulse : out std_logic
    );
end entity;

architecture rtl of debouncer is
    -- Counter that decreases at every clock if the signal is stable
    signal counter: unsigned (counter_size - 1 downto 0);
    -- Counter that decreases at every clock if the signal is stable
    signal candidate_value: std_logic;
    -- Stable value of bouncy input
    signal stable_value: std_logic;
    -- Used to create a pulse when stable_value changes
    signal delayed_stable_value: std_logic;
begin
    process (clk, reset)
    begin
        -- If signal is stable for a long time, then set stable_value to candidate_value
        if reset = '0' then
            counter <= ( others => '1' );
            candidate_value <= '0';
            stable_value <= '0';
        elsif rising_edge(clk) then
            if bouncy = candidate_value then
                if counter = 0 then
                    stable_value <= candidate_value;
                else
                    counter <= counter - 1;
                end if;
            else
                candidate_value <= bouncy;
                counter <= (others => '1');
            end if;
        end if;
    end process;
    -- Create delayed signal of stable_value so that we can detect changes
    -- (first derivative) and generate a single pulse
    process ( clk, reset )
    begin
        if reset = '0' then
            delayed_stable_value <= '0';
        elsif rising_edge( clk ) then
            delayed_stable_value <= stable_value;
        end if;
    end process;
    
    pulse <= '1' when stable_value = '1' and delayed_stable_value <= '0' else '0';

end architecture;
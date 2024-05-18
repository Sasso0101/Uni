library ieee;
use ieee.std_logic_1164.all;
use ieee.numeric_std.all;

entity debouncer is
    generic (
        counter_size : integer := 12
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
    signal candidate_value: std_logic;
    -- Stable value of bouncy input
    signal stable_value: std_logic;
    -- Used to create a pulse when stable_value changes
    signal delayed_stable_value: std_logic;
    signal count_reset: std_logic;
    signal count_enable: std_logic;
    signal tc: std_logic;
begin
    bit_counter: entity work.counter
    generic map (
        counter_size => counter_size
    )
    port map (
        clk => clk,
        reset => count_reset,
        count_enable => count_enable,
        tc => tc
    );
    process (clk, reset)
    begin
        if reset = '0' then
            candidate_value <= '0';
            delayed_stable_value <= '0';
            stable_value <= '0';
            count_enable <= '0';
            count_reset <= '1';
        elsif rising_edge(clk) then
            count_reset <= '1';
            -- Create delayed signal of stable_value so that we can detect changes and generate a single pulse
            delayed_stable_value <= stable_value;
            -- If signal is stable for a long time, then set stable_value to candidate_value
            if bouncy = candidate_value and tc = '1' then
                stable_value <= candidate_value;
                count_enable <= '0';
            elsif bouncy /= candidate_value then
                count_enable <= '1';
                candidate_value <= bouncy;
                count_reset <= '0';
            end if;
        end if;
    end process;

    pulse <= '1' when stable_value = '1' and delayed_stable_value <= '0' else '0';
end architecture;
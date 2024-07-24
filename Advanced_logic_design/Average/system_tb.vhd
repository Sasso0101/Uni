library ieee;
use ieee.std_logic_1164.all;
use ieee.numeric_std.all;

entity system_tb is
end entity;

architecture rtl of system_tb is
    component system is
        port (
            clk : in std_logic;
            command : in std_logic;
            reset: in std_logic;
            data_in: in std_logic_vector(7 downto 0);
            data_out: out std_logic
        );
    end component;
    signal command, clk, reset, data_out : std_logic := '0';
    signal data_in : std_logic_vector(7 downto 0) := "00000000";
begin
    system_0: system port map (
        command => command, clk => clk, reset => reset, data_in => data_in, data_out => data_out
    );

    clk <= not(clk) after 5 ns;

    process begin
        reset <= '1';
        wait for 10 ns;
        reset <= '0';
        command <= '1';
        data_in <= "10110011";
        wait for 10 ns;
        command <= '1';
        wait for 10 ns;
        command <= '0';
        wait for 10 ns;
        command <= '1';
        wait for 10 ns;
        command <= '0';
        wait for 100 ns;
        command <= '1';
        wait for 10 ns;
        command <= '1';
        wait for 10 ns;
        command <= '0';
        wait for 10 ns;
        command <= '1';
        wait;
    end process;
end architecture;
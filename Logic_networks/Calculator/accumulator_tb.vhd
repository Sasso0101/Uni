library ieee;
use ieee.std_logic_1164.all;
use ieee.numeric_std.all;

entity accumulator_tb is
end entity;

architecture rtl of accumulator_tb is
    component accumulator is
        port (
            clk, reset: in std_logic;
            acc_init, acc_enable: in std_logic;
            acc_in: in signed (31 downto 0);
            acc_out: out signed (31 downto 0)
        );
    end component;
    signal clk, reset: std_logic := '0';
    signal acc_init, acc_enable: std_logic := '0';
    signal acc_in: signed (31 downto 0) := (others => '0');
    signal acc_out: signed (31 downto 0) := (others => '0');
begin
    accumulator_0: accumulator port map ( clk => clk, reset => reset, acc_in => acc_in, acc_out => acc_out, acc_init => acc_init, acc_enable => acc_enable);

    clk <= not(clk) after 5 ns;

    process begin
        reset <= '0';
        wait for 10 ns;
        reset <= '1';
        acc_enable <= '1';
        acc_in <= to_signed(20, 32);
        wait for 10 ns;
        acc_enable <= '0';
        acc_in <= to_signed(30, 32);
        wait for 10 ns;
        acc_enable <= '1';
        wait for 10 ns;
        acc_init <= '1';
        wait for 10 ns;
        acc_init <= '0';
        wait for 10 ns;
        wait;
    end process;
end architecture;
library ieee;
use ieee.std_logic_1164.all;
use ieee.numeric_std.all;

entity square_tb is
end entity;

architecture testbench of square_tb is
    component square is
        port (
            n : in std_logic_vector( 2 downto 0 );
            s : out std_logic_vector( 5 downto 0 )
        );
    end component;
    signal n : std_logic_vector( 2 downto 0 );
    signal s : std_logic_vector( 5 downto 0 );
begin
    square_0: square port map (n => n, s => s);
    process begin
        n <= std_logic_vector(to_signed(0, 3));
        wait for 10 ns;
        n <= std_logic_vector(to_signed(1, 3));
        wait for 10 ns;
        n <= std_logic_vector(to_signed(2, 3));
        wait for 10 ns;
        n <= std_logic_vector(to_signed(3, 3));
        wait for 10 ns;
        n <= std_logic_vector(to_signed(-1, 3));
        wait for 10 ns;
        n <= std_logic_vector(to_signed(-2, 3));
        wait for 10 ns;
        n <= std_logic_vector(to_signed(-3, 3));
        wait for 10 ns;
        n <= std_logic_vector(to_signed(-4, 3));
        wait;
    end process;
end architecture testbench;
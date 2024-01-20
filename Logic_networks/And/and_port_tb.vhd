library IEEE;
use IEEE.std_logic_1164.all;
use IEEE.numeric_std.all;

entity and_port_tb is
end and_port_tb;

architecture Behavioral of and_port_tb is
    component and_port is
        port (
            x: in bit;
            y: in bit;
            z: out bit
        );
    end component;
    signal a, b, c: bit;
begin
    and_1: and_port port map (x => a, y => b, z => c);
    a <= '0', '1' after 10 ns, '0' after 20 ns;
    b <= '0', '1' after 15 ns, '0' after 20 ns;
end architecture Behavioral;
library ieee;

use IEEE.std_logic_1164.all;

entity and_port is
    Port (
        x, y: in bit;
        z: out bit
    );
end and_port;

architecture Behavioral of and_port is
begin
    z <= x and y;
end architecture Behavioral;
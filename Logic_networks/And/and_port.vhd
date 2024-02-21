library ieee;

use IEEE.std_logic_1164.all;

entity and_port is
    Port (
        x, y: in bit;
        z, w: buffer bit
    );
end and_port;

architecture rtl of and_port is
begin
    z <= not w or x;
    -- w <= not z;
end architecture rtl;
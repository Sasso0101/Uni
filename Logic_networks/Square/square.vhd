library ieee;

use ieee.std_logic_1164.all;

entity square is
    Port (
        n: in std_logic_vector(2 downto 0); -- Input number
        s: out std_logic_vector(5 downto 0) -- Squared number
    );
end Square;

architecture Behavioral of square is
begin
    s(5) <= '0';
    s(4) <= n(2) and not(n(1)) and not(n(0));
    s(3) <= (not(n(2)) and n(1) and n(0)) or (n(2) and not n(1) and n(0));
    s(2) <= (n(1) and not(n(0)));
    s(1) <= '0';
    s(0) <= n(0);
end architecture;
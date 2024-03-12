library ieee;
use ieee.std_logic_1164.all;
use ieee.numeric_std.all;

entity serializer is
    port (
        clk: in std_logic;
        start_ser: in std_logic;
        data_in: in std_logic_vector(7 downto 0);
        data_out: out std_logic;
        busy: out std_logic;
        reset: in std_logic
    );
end entity;

architecture rtl of serializer is
    signal counter: integer := 0;
begin
    process (clk, reset)
    begin
        if reset = '1' then
            busy <= '0';
            data_out <= '0';
            counter <= 0;
        elsif rising_edge(clk) then
            if start_ser = '1' or counter /= 0 then
                if counter = 8 then
                    counter <= 0;
                    busy <= '0';
                    data_out <= '0';
                else
                    busy <= '1';
                    data_out <= data_in(counter);
                    counter <= counter + 1;
                end if;
            end if;
        end if;
    end process;
end architecture;
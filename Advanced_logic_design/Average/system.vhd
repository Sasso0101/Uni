library ieee;
use ieee.std_logic_1164.all;
use ieee.numeric_std.all;

entity system is
    port (
        clk : in std_logic;
        command : in std_logic;
        reset: in std_logic;
        data_in: in std_logic_vector(7 downto 0);
        data_out: out std_logic
    );
end system;

architecture rtl of system is
    signal busy, start_ser: std_logic := '0';
begin
    recognizer : entity work.recognizer(rtl) 
    port map (
        clk => clk,
        command => command,
        start_ser => start_ser,
        busy => busy,
        reset => reset
    );
    serializer : entity work.serializer(rtl) 
    port map (
        clk => clk,
        start_ser => start_ser,
        data_in => data_in,
        data_out => data_out,
        busy => busy,
        reset => reset
    );
end architecture;
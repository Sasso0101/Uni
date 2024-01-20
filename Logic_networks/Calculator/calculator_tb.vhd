library ieee;
use ieee.std_logic_1164.all;


entity calculator_tb is
end entity;

architecture rtl of calculator_tb is
    component calculator is
        port (
            in1, in2, in3: in std_logic;
            out1, out2, out3, out4: out std_logic
        );
    end component;
    signal in1, in2, in3, out1, out2, out3, out4: std_logic;
begin
    calculator_0: calculator port map (in1 => in1, in2 => in2, in3 => in3, out1 => out1, out2 => out2, out3 => out3, out4 => out4);
    process begin
        in1 <= '0', '1' after 10 ns, '0' after 20 ns, '1' after 40 ns;
        in2 <= '0', '1' after 10 ns, '0' after 20 ns, '1' after 50 ns;
        in3 <= '0', '1' after 10 ns, '0' after 30 ns;
        wait for 60 ns;
        wait;
    end process;
end architecture;
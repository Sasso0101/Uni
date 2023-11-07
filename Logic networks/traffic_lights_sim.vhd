library ieee;
use ieee.std_logic_1164.all;

entity tb_add is
end tb_add;

architecture tb of tb_add is

    component add
        port (b0 : in STD_LOGIC;
           b1 : in STD_LOGIC;
           b2 : in STD_LOGIC;
           b3 : in STD_LOGIC;
           red : out STD_LOGIC;
           blue : out STD_LOGIC);
    end component;

    signal b0 : std_logic;
    signal b1 : std_logic;
    signal b2 : std_logic;
    signal b3 : std_logic;
    signal red: std_logic;
    signal blue: std_logic;

begin

    dut : add
    port map (b0 => b0,
              b1 => b1,
              b2 => b2,
              b3 => b3,
              red => red,
              blue => blue);

    stimuli : process
    begin
        -- EDIT Adapt initialization as needed
        b0 <= '0';
        b1 <= '0';
        b2 <= '0';
        b3 <= '1';
        wait for 10 ns;
        b0 <= '0';
        b1 <= '0';
        b2 <= '1';
        b3 <= '0';
        wait for 10 ns;
        b0 <= '0';
        b1 <= '1';
        b2 <= '0';
        b3 <= '0';
        wait for 10 ns;
        b0 <= '1';
        b1 <= '0';
        b2 <= '0';
        b3 <= '0';
        wait for 10 ns;
        wait;
    end process;

end tb;

-- Configuration block below is required by some simulators. Usually no need to edit.

configuration cfg_tb_add of tb_add is
    for tb
    end for;
end cfg_tb_add;
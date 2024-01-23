library ieee;
use ieee.std_logic_1164.all;
use ieee.numeric_std.all;

entity calculator_tb is
end entity;

architecture rtl of calculator_tb is
    signal clk : std_logic := '0';
    signal sw : std_logic_vector( 15 downto 0 );
    signal led : std_logic_vector( 15 downto 0 );
    signal btnC, btnU, btnL, btnR, btnD : std_logic := '0';
    signal ca, cb, cc, cd, ce, cf, cg, dp : std_logic;
    signal an : std_logic_vector( 3 downto 0 );
begin
    dut : entity work.calculator(rtl) port map (
        CLK => clk,
        SW => sw,
        LED => led,
        CA => ca, CB => cb, CC => cc, CD => cd, CE => ce, CF => cf, CG => cg, DP => dp,
        AN => an,
        BTNC => btnC, BTNU => btnU, BTNL => btnL, BTNR => btnR, BTND => btnD
    );

    clk <= not(clk) after 5 ns;

    process begin
        sw <= (others => '0');
        BTNC <= '0';
        BTNU <= '0';
        BTNL <= '0';
        BTNR <= '0';
        BTND <= '0';
        sw <= ('1', others => '0');
        wait for 20 ns;
        sw <= (others => '0');
        -- Set number '41' on the switches
        sw <= std_logic_vector(to_signed(5, 16));
        wait for 20 ns;
        -- Load value
        btnU <= '1';
        wait for 100 ns;
        btnU <= '0';
        wait for 100 ns;

        -- set number '6' on the switches
        sw <= std_logic_vector(to_unsigned(6, 16));
        -- Perform sum (click on the up button)
        btnL <= '1';
        wait for 100 ns;
        btnL <= '0';
        wait for 100 ns;
        -- Perform a single multiplication (click on the right button)
        -- btnR <= '1';
        -- wait for 100 ns;
        -- btnR <= '0';
        wait;
    end process;
end architecture;
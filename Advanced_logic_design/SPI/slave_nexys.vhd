library ieee;
use ieee.std_logic_1164.all;
use ieee.numeric_std.all;

entity slave_nexys is
    port (
        clk : in std_logic;
        sw : in  STD_LOGIC_VECTOR (15 downto 0); -- Switches
        led : out  STD_LOGIC_VECTOR (15 downto 0); -- leds
        btnL, btnD : in std_logic; -- Buttons
        JA(3), JA(4), JA(10) : in std_logic; -- MOSI, SCK, SS
        JA(9) : out std_logic -- MISO
    );
end slave_nexys;

architecture rtl of slave_nexys is
    signal pulseL, pulseD : std_logic;
    signal data_tx, data_rx : std_logic_vector(7 downto 0);
    type t_Display is (Tx, Rx);
    signal display : t_Display := Tx;
begin
    -- Initialize buttons
    left_detect : entity work.debouncer(rtl) 
    port map (
        clk => clk,
        reset   => sw(15),
        bouncy  => btnL,
        pulse   => pulseL
    );
    down_detect : entity work.debouncer(rtl) 
    port map (
        clk => clk,
        reset   => sw(15),
        bouncy  => btnD,
        pulse   => pulseD
    );

    spi_slave_0: entity work.spi_slave
    generic map (
      CPHA => '1'
    )
    port map (
        clk => clk,
        reset => sw(15),
        load_data => pulseL,
        data => sw(7 downto 0),
        data_tx => data_tx,
        data_rx => data_rx,
        MOSI => JA3,
        MISO => JA9,
        SCK => JA4,
        SS => JA10
    );

    process (clk)
    begin
        if rising_edge(clk) and pulseD = '1' then
            display <= Tx when display = Rx else Rx;
        end if;
    end process;

    led <= "00000000" & data_tx when display = Tx else "00000000" & data_rx;
end architecture;
library ieee;
use ieee.std_logic_1164.all;
use ieee.numeric_std.all;

entity master_nexys is
    port (
        clk : in std_logic;
        sw : in  STD_LOGIC_VECTOR (15 downto 0); -- Switches
        led : out  STD_LOGIC_VECTOR (15 downto 0); -- leds
        btnC, btnL, btnD : in std_logic; -- Buttons
        JA1, JA2, JA8 : out std_logic; -- MOSI, SCK, SS
        JA7 : in std_logic -- MISO
    );
end master_nexys;

architecture rtl of master_nexys is
    signal pulseC, pulseL, pulseD : std_logic;
    signal data_tx, data_rx : std_logic_vector(7 downto 0);
    type t_Display is (Tx, Rx);
    signal display : t_Display := Tx;
begin
    -- Initialize buttons
    center_detect : entity work.debouncer(rtl) 
    port map (
        clk => clk,
        reset   => sw(15),
        bouncy  => btnC,
        pulse   => pulseC
    );
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

    spi_master_0: entity work.spi_master
    generic map (
      CPOL => '1',
      CPHA => '1'
    )
    port map (
        clk => clk,
        reset => sw(15),
        load_data => pulseL,
        data => sw(7 downto 0),
        data_tx => data_tx,
        data_rx => data_rx,
        start => pulseC,
        MOSI => JA1,
        MISO => JA7,
        SCK => JA2,
        SS => JA8
    );

    process (clk)
    begin
        if rising_edge(clk) and pulseD = '1' then
            if display = Rx then
                display <= Tx;
            else
                display <= Rx;
            end if;
        end if;
    end process;

    LED <= "00000000" & data_tx when display = Tx else "00000000" & data_rx;
end architecture;
library ieee;
use ieee.std_logic_1164.all;
use ieee.numeric_std.all;

entity master_nexys is
    port (
        CLK100MHZ : in std_logic;
        SW : in  std_logic_vector(15 downto 0); -- Switches
        LED : out  std_logic_vector(15 downto 0); -- Leds
        CPU_RESETN : in std_logic; -- Reset
        BTNC, BTNL, BTND, BTNR : in std_logic; -- Buttons
        JA1, JA2, JA8 : out std_logic; -- Master: MOSI, SCK, SS
        JA7 : in std_logic; -- Master: MISO
        JA3, JA4, JA10 : in std_logic; -- Slave: MOSI, SCK, SS
        JA9 : out std_logic -- Slave: MISO
    );
end master_nexys;

architecture rtl of master_nexys is
    signal pulseC, pulseL, pulseD, pulseR : std_logic := '0';
    signal master_data_tx, master_data_rx, slave_data_tx, slave_data_rx : std_logic_vector(7 downto 0) := (others => '0');
    type t_Display is (Tx, Rx);
    signal display : t_Display := Tx;
begin
    -- Initialize buttons
    center_detect : entity work.debouncer(rtl) 
    port map (
        clk => CLK100MHZ,
        reset   => CPU_RESETN,
        bouncy  => BTNC,
        pulse   => pulseC
    );
    left_detect : entity work.debouncer(rtl) 
    port map (
        clk => CLK100MHZ,
        reset   => CPU_RESETN,
        bouncy  => BTNL,
        pulse   => pulseL
    );
    right_detect : entity work.debouncer(rtl) 
    port map (
        clk => CLK100MHZ,
        reset   => CPU_RESETN,
        bouncy  => BTNR,
        pulse   => pulseR
    );
    down_detect : entity work.debouncer(rtl) 
    port map (
        clk => CLK100MHZ,
        reset   => CPU_RESETN,
        bouncy  => BTND,
        pulse   => pulseD
    );

    spi_master_0: entity work.spi_master
    generic map (
      CPOL => '1',
      CPHA => '1'
    )
    port map (
        clk => CLK100MHZ,
        reset => CPU_RESETN,
        load_data => pulseL,
        data => SW(15 downto 8),
        data_tx => master_data_tx,
        data_rx => master_data_rx,
        start => pulseC,
        MOSI => JA1,
        MISO => JA7,
        SCK => JA2,
        SS => JA8
    );
    spi_slave_0: entity work.spi_slave
    generic map (
      CPHA => '1'
    )
    port map (
        clk => CLK100MHZ,
        reset => CPU_RESETN,
        load_data => pulseR,
        data => sw(7 downto 0),
        data_tx => slave_data_tx,
        data_rx => slave_data_rx,
        MOSI => JA3,
        MISO => JA9,
        SCK => JA4,
        SS => JA10
    );

    process (CLK100MHZ)
    begin
        if rising_edge(CLK100MHZ) then
            if pulseD = '1' then
                if display = Rx then
                    display <= Tx;
                else
                    display <= Rx;
                end if;
            end if;
        end if;
    end process;
    LED(15 downto 0) <= master_data_tx & slave_data_tx when display = Tx else master_data_rx & slave_data_rx;
end architecture;
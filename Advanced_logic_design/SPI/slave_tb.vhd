-- Library declarations
library ieee;
use ieee.std_logic_1164.all;
use ieee.numeric_std.all;
-- A generic Master SPI interface
entity spi_slave_tb is
end entity spi_slave_tb;

architecture rtl of spi_slave_tb is
  signal clk: std_logic := '0';
  signal reset: std_logic := '0';
  signal load_data: std_logic := '0';
  signal data: std_logic_vector(7 downto 0) := (others => '0') ;
  signal data_tx: std_logic_vector( 7 downto 0 ) := (others => '0') ;
  signal data_rx: std_logic_vector( 7 downto 0 ) := (others => '0') ;
  signal MOSI : std_logic := '0';
  signal MISO: std_logic := '0';
  signal SCK: std_logic := '0';
  signal SS: std_logic := '0';
begin
  spi_slave_0: entity work.spi_slave
  generic map (
    CPHA => '1'
  )
  port map (
    clk => clk,
    reset => reset,
    load_data => load_data,
    data => data,
    data_tx => data_tx,
    data_rx => data_rx,
    MOSI => MOSI,
    MISO => MISO,
    SCK => SCK,
    SS => SS
  );

  clk <= not(clk) after 100 ns;
  process begin
    SCK <= '1';
    SS <= '1';
    reset <= '1';
    wait for 10 us;
    reset <= '0';
    data <= "10001001";
    load_data <= '1';
    wait for 10 us;
    load_data <= '0';
    wait for 10 us;

    SS <= '0';
    wait for 3.2 us; -- For CPHA = 1
    MOSI <= '1';
    SCK <= '0'; -- For CPHA = 1
    -- SCK <= '1'; -- For CPHA = 0
    wait for 3.2 us;
    SCK <= not(SCK);
    wait for 3.2 us;

    SCK <= not(SCK);
    MOSI <= '0';
    wait for 3.2 us;
    SCK <= not(SCK);
    wait for 3.2 us;

    SCK <= not(SCK);
    MOSI <= '0';
    wait for 3.2 us;
    SCK <= not(SCK);
    wait for 3.2 us;

    SCK <= not(SCK);
    MOSI <= '1';
    wait for 3.2 us;
    SCK <= not(SCK);
    wait for 3.2 us;

    SCK <= not(SCK);
    MOSI <= '0';
    wait for 3.2 us;
    SCK <= not(SCK);
    wait for 3.2 us;

    SCK <= not(SCK);
    MOSI <= '1';
    wait for 3.2 us;
    SCK <= not(SCK);
    wait for 3.2 us;

    SCK <= not(SCK);
    MOSI <= '0';
    wait for 3.2 us;
    SCK <= not(SCK);
    wait for 3.2 us;

    SCK <= not(SCK);
    MOSI <= '1';
    wait for 3.2 us;
    SCK <= not(SCK);
    wait for 3.2 us;
    SS <= '1';
    SCK <= '1';
    wait;
  end process;
end architecture;
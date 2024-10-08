library ieee;
use ieee.std_logic_1164.all;
use ieee.numeric_std.all;

entity spi_master_tb is
end entity;

architecture rtl of spi_master_tb is

  signal clk : std_logic := '0';
  signal reset : std_logic := '0';
  signal load_data : std_logic := '0';
  signal data : std_logic_vector( 7 downto 0 ) := (others => '0');
  signal data_tx : std_logic_vector( 7 downto 0 ) := (others => '0');
  signal data_rx : std_logic_vector( 7 downto 0 ) := (others => '0');
  signal start : std_logic := '0';
  signal MOSI : std_logic := '0';
  signal MISO : std_logic := '0';
  signal SCK : std_logic := '0';
  signal SS : std_logic := '0';
begin
    spi_master_0: entity work.spi_master
    generic map (
      CPOL => '0',
      CPHA => '0'
    )
    port map (
        clk => clk,
        reset => reset,
        load_data => load_data,
        data => data,
        data_tx => data_tx,
        data_rx => data_rx,
        start => start,
        MOSI => MOSI,
        MISO => MISO,
        SCK => SCK,
        SS => SS
    );

    clk <= not(clk) after 100 ns;
    process begin
        reset <= '1';
        wait for 10 us;
        reset <= '0';
        data <= "10001001";
        load_data <= '1';
        wait for 10 us;
        load_data <= '0';
        start <= '1';
        wait for 300 ns; -- for CPHA = 0
        --wait for 3.7 us; -- for CPHA = 1
        start <= '0';
        MISO <= '1';
        wait for 6.6 us;
        MISO <= '0';
        wait for 6.4 us;
        MISO <= '1';
        wait for 6.4 us;
        MISO <= '1';
        wait for 6.4 us;
        MISO <= '0';
        wait for 6.4 us;
        MISO <= '1';
        wait for 6.4 us;
        MISO <= '0';
        wait for 6.4 us;
        MISO <= '1';
        wait for 6.4 us;
        wait;
    end process;
end architecture;
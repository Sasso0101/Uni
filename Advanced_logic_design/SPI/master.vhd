-- Library declarations
library ieee;
use ieee.std_logic_1164.all;
use ieee.numeric_std.all;
-- A generic Master SPI interface
entity spi_master is
  generic (
    -- Number of bits to transfer
    NUM  : natural   := 8;
    CPOL : std_logic := '1';
    CPHA : std_logic := '1'
  );
  port (
    -- System clock
    clk : in std_logic;
    -- System reset (active low)
    reset : in std_logic;
    -- Command: load the data to be sent
    load_data : in std_logic;
    -- Data: the data to be sent
    data : in std_logic_vector(7 downto 0);
    -- Data the be sent currently in shift register
    data_tx : inout std_logic_vector( 7 downto 0 );
    -- Data received
    data_rx : inout std_logic_vector( 7 downto 0 );
    -- Command: start data transmission
    start : in std_logic;
    -- SPI interface signals (Master)
    MOSI : out std_logic;
    MISO : in std_logic;
    SCK  : out std_logic;
    SS   : out std_logic
  );
end entity spi_master;

architecture rtl of spi_master is
  type t_State is (Idle, Transmitting, Receiving);
  signal state : t_State := Idle;
  signal enable_tx: std_logic := '0';
  signal enable_rx: std_logic := '0';
  signal bit_count_tc: std_logic := '0';
  signal bit_count_en: std_logic := '0';
  signal clk_count_tc: std_logic := '0';
  signal clk_count_en: std_logic := '0';
  signal chip_enable: std_logic := '0';
begin
  register_tx : entity work.shift_reg
  port map (
    clk => clk,
    reset => reset,
    shift_enable => enable_tx,
    chip_enable => chip_enable,
    load_enable => load_data,
    parallel_in => data,
    data => data_tx,
    data_in => '0',
    data_out => MOSI
  );
  register_rx : entity work.shift_reg
  port map (
    clk => clk,
    reset => reset,
    shift_enable => enable_rx,
    chip_enable => chip_enable,
    load_enable => '0',
    parallel_in => (others => '0') ,
    data => data_rx,
    data_in => MISO,
    data_out => open
  );
  bit_counter: entity work.counter
  generic map (
    counter_size => 3
  )
  port map (
    clk => clk,
    reset => reset,
    count_enable => bit_count_en,
    tc => bit_count_tc
  );
  clk_counter: entity work.counter
  generic map (
    counter_size => 4
  )
  port map (
    clk => clk,
    reset => reset,
    count_enable => clk_count_en,
    tc => clk_count_tc
  );

  process ( clk, reset )
  begin    
    if reset = '1' then
      enable_rx <= '0';
      chip_enable <= '0';
      bit_count_en <= '0';
      enable_tx <= '0';
      clk_count_en <= '0';
      SCK <= CPOL;
      SS <= '1';
      MOSI <= 'Z';
      state <= Idle;
    else
      if rising_edge (clk) then
        if bit_count_tc = '1' and enable_tx = '1' then
          enable_tx <= '0';
          chip_enable <= '0';
          enable_rx <= '0';
          bit_count_en <= '0';
          clk_count_en <= '0';
          MOSI <= 'Z';
          SS <= '1';
          SCK <= CPOL;
          state <= Idle;
        elsif state = Receiving then
          SS <= '0';
          chip_enable <= '1';
          if (CPHA = '0' and CPOL = '1') or (CPHA = '1' and CPOL = '0') then
            SCK <= '0';
          else
            SCK <= '1';
          end if;
          bit_count_en <= '0';
          enable_rx <= '0';
          if clk_count_tc = '1' then
            bit_count_en <= '1';
            enable_tx <= '1';
            state <= Transmitting;
          end if;
        elsif state = Transmitting then
          bit_count_en <= '0';
          chip_enable <= '1';
          SS <= '0';
          if (CPHA = '0' and CPOL = '0') or (CPHA = '1' and CPOL = '1') then
            SCK <= '0';
          else
            SCK <= '1';
          end if;
          enable_tx <= '0';
          if clk_count_tc = '1' then
            enable_rx <= '1';
            state <= Receiving;
          end if;
        elsif State = Idle and start = '1' then
          clk_count_en <= '1';
          if CPHA = '0' then
            bit_count_en <= '1';
            enable_tx <= '1';
            state <= Transmitting;
          else
            state <= Receiving;
          end if;
        end if;
      end if;
    end if;
  end process;
end architecture;
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
    data_tx : out std_logic_vector( 7 downto 0 );
    -- Data received
    data_rx : out std_logic_vector( 7 downto 0 );
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
  type t_State is (Idle, Transceiving);
  signal state : t_State := Idle;
  signal clk_tx: std_logic := '0';
  signal clk_rx: std_logic := '0';
  signal data_out_tx: std_logic := '0';
  signal bit_count_tc: std_logic := '0';
  signal bit_count_en: std_logic := '0';
  signal clk_count_tc: std_logic := '0';
  signal clk_count_en: std_logic := '0';
begin
  register_tx : entity work.shift_reg
  port map (
    clk => clk_tx,
    reset => reset,
    shift_enable => '1',
    load_enable => load_data,
    parallel_in => data,
    data => data_tx,
    data_in => '0',
    data_out => data_out_tx
  );
  register_rx : entity work.shift_reg
  port map (
    clk => clk_rx,
    reset => reset,
    shift_enable => '1',
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
    clk => clk_tx,
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

  MOSI <= data_out_tx when state = Transceiving else 'Z';

  process ( clk, reset )
  begin    
    if reset = '1' then
      clk_tx <= '0';
      clk_rx <= '0';
      clk_count_en <= '0';
      SCK <= CPOL;
      SS <= '1';
      MOSI <= 'Z';
      state <= Idle;
    else
      if rising_edge (clk) then
        if state = Transceiving and clk_count_tc = '1' then
          clk_tx <= not(clk_tx);
          clk_rx <= not(clk_rx);
          SCK <= not(SCK);
          if bit_count_tc = '1' and clk_tx = '0' then
            clk_count_en <= '0';
            bit_count_en <= '0';
            clk_tx <= '0';
            clk_rx <= '0';
            MOSI <= 'Z';
            SS <= '1';
            SCK <= CPOL;
            state <= Idle;
          end if;
        elsif State = Idle and start = '1' then
          clk_count_en <= '1';
          bit_count_en <= '1';
          state <= Transceiving;
          SS <= '0';
          clk_tx <= '1' when CPHA = '0' else '0';
          clk_rx <= '0' when CPHA = '0' else '1';
        end if;
      end if;
    end if;
  end process;
end architecture;
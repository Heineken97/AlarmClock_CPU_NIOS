	component CPU_Nios is
		port (
			buttons_export            : in  std_logic_vector(3 downto 0) := (others => 'X'); -- export
			buzzer_export             : out std_logic;                                       -- export
			clk_clk                   : in  std_logic                    := 'X';             -- clk
			leds_export               : out std_logic_vector(9 downto 0);                    -- export
			sevseg_hours_tens_export  : out std_logic_vector(6 downto 0);                    -- export
			sevseg_hours_units_export : out std_logic_vector(6 downto 0);                    -- export
			sevseg_mins_tens_export   : out std_logic_vector(6 downto 0);                    -- export
			sevseg_mins_units_export  : out std_logic_vector(6 downto 0);                    -- export
			sevseg_secs_tens_export   : out std_logic_vector(6 downto 0);                    -- export
			sevseg_secs_units_export  : out std_logic_vector(6 downto 0);                    -- export
			switches_export           : in  std_logic_vector(1 downto 0) := (others => 'X')  -- export
		);
	end component CPU_Nios;

	u0 : component CPU_Nios
		port map (
			buttons_export            => CONNECTED_TO_buttons_export,            --            buttons.export
			buzzer_export             => CONNECTED_TO_buzzer_export,             --             buzzer.export
			clk_clk                   => CONNECTED_TO_clk_clk,                   --                clk.clk
			leds_export               => CONNECTED_TO_leds_export,               --               leds.export
			sevseg_hours_tens_export  => CONNECTED_TO_sevseg_hours_tens_export,  --  sevseg_hours_tens.export
			sevseg_hours_units_export => CONNECTED_TO_sevseg_hours_units_export, -- sevseg_hours_units.export
			sevseg_mins_tens_export   => CONNECTED_TO_sevseg_mins_tens_export,   --   sevseg_mins_tens.export
			sevseg_mins_units_export  => CONNECTED_TO_sevseg_mins_units_export,  --  sevseg_mins_units.export
			sevseg_secs_tens_export   => CONNECTED_TO_sevseg_secs_tens_export,   --   sevseg_secs_tens.export
			sevseg_secs_units_export  => CONNECTED_TO_sevseg_secs_units_export,  --  sevseg_secs_units.export
			switches_export           => CONNECTED_TO_switches_export            --           switches.export
		);


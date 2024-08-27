
module CPU_Nios (
	buttons_export,
	buzzer_export,
	clk_clk,
	leds_export,
	sevseg_hours_tens_export,
	sevseg_hours_units_export,
	sevseg_mins_tens_export,
	sevseg_mins_units_export,
	sevseg_secs_tens_export,
	sevseg_secs_units_export,
	switches_export);	

	input	[3:0]	buttons_export;
	output		buzzer_export;
	input		clk_clk;
	output	[9:0]	leds_export;
	output	[6:0]	sevseg_hours_tens_export;
	output	[6:0]	sevseg_hours_units_export;
	output	[6:0]	sevseg_mins_tens_export;
	output	[6:0]	sevseg_mins_units_export;
	output	[6:0]	sevseg_secs_tens_export;
	output	[6:0]	sevseg_secs_units_export;
	input	[1:0]	switches_export;
endmodule

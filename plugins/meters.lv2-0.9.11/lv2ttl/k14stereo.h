// generated by lv2ttl2c from
// http://gareus.org/oss/lv2/meters#K14stereo

extern const LV2_Descriptor* lv2_descriptor(uint32_t index);
extern const LV2UI_Descriptor* lv2ui_kmeter(uint32_t index);

static const RtkLv2Description _plugin_k14stereo = {
	&lv2_descriptor,
	&lv2ui_kmeter
	, 20 // uint32_t dsp_descriptor_id
	, 0 // uint32_t gui_descriptor_id
	, "K14/RMS Meter (Stereo)" // const char *plugin_human_id
	, (const struct LV2Port[10])
	{
		{ "ref", CONTROL_IN, -4.000000, -4.000000, 4.000000, "ui notification"},
		{ "inL", AUDIO_IN, nan, nan, nan, "InL"},
		{ "outL", AUDIO_OUT, nan, nan, nan, "OutL"},
		{ "levelL", CONTROL_OUT, nan, 0.000000, 1.000000, "LevelL"},
		{ "inR", AUDIO_IN, nan, nan, nan, "InR"},
		{ "outR", AUDIO_OUT, nan, nan, nan, "OutR"},
		{ "levelR", CONTROL_OUT, nan, 0.000000, 1.000000, "LevelR"},
		{ "peakL", CONTROL_OUT, nan, 0.000000, 1.000000, "peakL"},
		{ "peakR", CONTROL_OUT, nan, 0.000000, 1.000000, "peakR"},
		{ "hold", CONTROL_OUT, nan, 0.000000, 1.000000, "peak hold"},
	}
	, 10 // uint32_t nports_total
	, 2 // uint32_t nports_audio_in
	, 2 // uint32_t nports_audio_out
	, 0 // uint32_t nports_midi_in
	, 0 // uint32_t nports_midi_out
	, 0 // uint32_t nports_atom_in
	, 0 // uint32_t nports_atom_out
	, 6 // uint32_t nports_ctrl
	, 1 // uint32_t nports_ctrl_in
	, 5 // uint32_t nports_ctrl_out
	, 8192 // uint32_t min_atom_bufsiz
	, false // bool send_time_info
	, UINT32_MAX // uint32_t latency_ctrl_port
};

#ifdef X42_PLUGIN_STRUCT
#undef X42_PLUGIN_STRUCT
#endif
#define X42_PLUGIN_STRUCT _plugin_k14stereo
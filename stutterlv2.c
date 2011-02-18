/* -*- Mode: C; indent-tabs-mode: t; c-basic-offset: 4; tab-width: 4 -*- */
/*
 * main.c - Auto-generated by Anjuta's Makefile project wizard
 * 
 */

#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <math.h>

#include <lv2.h>

#define STUT_URI       "urn:jeremy.salwen:plugins:stutter"

#define STUT_MIX      0
#define STUT_BPM      1
#define STUT_DIVISION      2
#define STUT_INPUT     3
#define STUT_OUTPUT    4

typedef struct {
	float * mix;
	float * bpm;
	float * division;
	float * input;
	float * output;

	float coef;
	double phase;
	double s_rate;
} Stut;

static void cleanupStut(LV2_Handle instance)
{
	free(instance);
}

static void connectPortStut(LV2_Handle instance, uint32_t port,
			   void *data)
{
	Stut *plugin = (Stut *)instance;

	switch (port) {
		case STUT_MIX:
			plugin->mix = data;
			break;
		case STUT_INPUT:
			plugin->input = data;
			break;
		case STUT_OUTPUT:
			plugin->output = data;
			break;
		case STUT_BPM:
			plugin->bpm=data;
			break;
		case STUT_DIVISION:
			plugin->division=data;
			break;
	}
}

static LV2_Handle instantiateStut(const LV2_Descriptor *descriptor,
	    double s_rate, const char *path,
	    const LV2_Feature * const* features)
{
	Stut *plugin_data = (Stut*)malloc(sizeof(Stut));
	plugin_data->s_rate=s_rate;
	plugin_data->phase=0;
	plugin_data->coef=0;
	return (LV2_Handle)plugin_data;
}

static void runStut(LV2_Handle instance, uint32_t sample_count)
{
	Stut *plugin_data = (Stut *)instance;

	const float mix = 1-*(plugin_data->mix);
	const double bpm= *(plugin_data->bpm);
	const unsigned int division=*(plugin_data->division);
	const double beatlength=(plugin_data->s_rate*60)/bpm;
	double divisor;
	switch(division) {
		case 0: divisor=1/4; break;
		case 1: divisor=1/3; break;
		case 2: divisor=1/2; break;
		case 3: divisor=1; break;
		case 4: divisor=2; break;
		case 5: divisor=3; break;
		case 6: divisor=4; break;
		case 7: divisor=6; break;
		case 8: divisor=8; break;
		case 9: divisor=9; break;
		case 10: divisor=12; break;
		case 11: divisor=16; break;
		case 12: divisor=24; break;
		case 13: divisor=32; break;
		case 14: divisor=48; break;
		case 15: divisor=64; break;
		default: divisor=1; break;
	}
	const double stutterlength=beatlength/divisor;
	
	const float * const input = plugin_data->input;
	float * const output = plugin_data->output;

	float coef=plugin_data->coef;
	if(coef<1) {
		coef=mix;
	}
	//To satisfy a*x+b=1 and a*1+b=x, we get a=-1, b=1+x
	float twiddle=1+mix;
	double phase=plugin_data->phase;
	unsigned int pos=0;
	while(1) {
		unsigned int rest=floor(stutterlength-phase)+1;
		if(pos+rest>sample_count) {
			break;
		}
		//Pulling out these variables to make GCC vectorize on x86_64
		const float* inbuf=input+pos;
		float* outbuf=output+pos;
		for(unsigned int i=0;i<rest; i++) {
			outbuf[i] = inbuf[i] * coef;
		}
		coef=twiddle-coef;
		
		pos+=rest;
		phase+=rest-stutterlength;
	}

	//To make GCC auto-vectorize
	const float* inbuf=input+pos;
	float* outbuf=output+pos;
	for(unsigned int i=0; i<sample_count-pos; i++) {
		outbuf[i] = inbuf[i] * coef;
	}
	plugin_data->phase=phase+(sample_count-pos);
	plugin_data->coef=coef;
}

static LV2_Descriptor stutDescriptor = 
{
	.URI = STUT_URI,
	.activate = NULL,
	.cleanup = cleanupStut,
	.connect_port = connectPortStut,
	.deactivate = NULL,
	.instantiate = instantiateStut,
	.run = runStut,
	.extension_data = NULL,
};

LV2_SYMBOL_EXPORT const LV2_Descriptor *lv2_descriptor(uint32_t index)
{

	switch (index) {
	case 0:
		return &stutDescriptor;
	default:
		return NULL;
	}
}

#ifndef IO_functionsH
#define IO_functionsH
//---------------------------------------------------------------------------

#include<string>
#include "parameter_classes.h"

using namespace std;

//---------------------------------------------------------------------------

void parse_config(string namefile_config, string& read_images, string& images_path, string& search_str_images, 
	string& process_read_images, string& visualize_read_images, string& create_artif_images, string& artif_images_path, 
	artif_im_param*& artif_im_param_read, string& save_artif_images, string& create_synth_images, string& text_mod_path, 
	string& search_str_text_mod, string& text_mod_res_path, text_mod_param*& text_mod_param_read, string& save_synth_images,
	string& compute_Haralick_feat, string& Haralick_path, string& search_str_images_Haralick, string& res_Haralick_path, 
	int*& Haralick_smaller_wndw, Haralick_param*& Haralick_param_read, string& save_Haralick_feat);
void search_filenames(list <string>& filename_list, string search_string, string namefile1_path, bool not_found);
void parse_txt_by_space(string namefile, string*& word_list, int& num_words);
void parse_txt_by_space(string namefile, double*& word_list, int& num_words);

#endif
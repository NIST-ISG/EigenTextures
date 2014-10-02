//---------------------------------------------------------------------------

#include<iostream>
#include<istream>
#include<fstream>
#include<cmath>
#include<string>
//#include<sstream>
#include<list>
#include <Windows.h>
#include "parameter_classes.h"
//#pragma hdrstop

#include "IO_functions.h"

using namespace std;

void parse_config(string namefile_config, string& read_images, string& images_path, string& search_str_images, 
	string& process_read_images, string& visualize_read_images, string& create_artif_images, string& artif_images_path, 
	artif_im_param*& artif_im_param_read, string& save_artif_images, string& create_synth_images, string& text_mod_path, 
	string& search_str_text_mod, string& text_mod_res_path, text_mod_param*& text_mod_param_read, string& save_synth_images,
	string& compute_Haralick_feat, string& Haralick_path, string& search_str_images_Haralick, string& res_Haralick_path, 
	int*& Haralick_smaller_wndw, Haralick_param*& Haralick_param_read, string& save_Haralick_feat)
{
    const char* namefile_ch = new char[50];
    namefile_ch = namefile_config.c_str();

    ifstream infile;
    infile.open(namefile_ch);

    if (!infile)
    {
        cout << "File not found\n";
    }

    if ((infile)&&(!infile.is_open()))
    {
        cout << "File not open\n";
    }
	string temp;
	infile >> read_images; infile >> temp;
	infile >> images_path; infile >> temp;
	infile >> search_str_images; infile >> temp;
	infile >> process_read_images; infile >> temp;
	infile >> visualize_read_images; infile >> temp;
	infile >> create_artif_images; infile >> temp;
	infile >> artif_images_path; infile >> temp;
	
	infile >> temp; //artif images parameters BEGINNING
	
	int im_dim=0; int* im_size = NULL; int num_saltpepper_param_within=0; int num_saltpepper_param=0; int** saltpepper_param=NULL; 
	int num_Gauss_param_within=0; int num_Gauss_param=0; double** Gauss_param=NULL; int num_kern_param=0; int** kern_param=NULL; 
	int num_angs_kern=0; double* ang_kern=NULL;
	infile >> temp; im_dim = atoi(temp.c_str()); im_size = new int[im_dim];
	for (int i=0; i<im_dim; i++) {infile >> temp; *(im_size+i) = atoi(temp.c_str());}
	infile >> temp;
	infile >> temp; num_saltpepper_param_within = atoi(temp.c_str());
	infile >> temp; num_saltpepper_param = atoi(temp.c_str()); saltpepper_param = new int*[num_saltpepper_param];
	for (int i=0; i<num_saltpepper_param; i++) 
	{
		*(saltpepper_param+i) = new int[num_saltpepper_param_within];
		for (int j=0; j<num_saltpepper_param_within; j++) { infile >> temp; *(*(saltpepper_param+i)+j) = atoi(temp.c_str()); }
	}
	infile >> temp;
	infile >> temp; num_Gauss_param_within = atoi(temp.c_str());
	infile >> temp; num_Gauss_param = atoi(temp.c_str()); Gauss_param = new double*[num_Gauss_param];
	for (int i=0; i<num_Gauss_param; i++) 
	{
		*(Gauss_param+i) = new double[num_Gauss_param_within];
		for (int j=0; j<num_Gauss_param_within; j++) { infile >> temp; *(*(Gauss_param+i)+j) = atof(temp.c_str()); }
	}
	infile >> temp;
	infile >> temp; num_kern_param = atoi(temp.c_str()); kern_param = new int*[num_kern_param];
	for (int i=0; i<num_kern_param; i++) 
	{
		*(kern_param+i) = new int[4];
		for (int j=0; j<4; j++) { infile >> temp; *(*(kern_param+i)+j) = atoi(temp.c_str()); }
	}
	infile >> temp;
	infile >> temp; num_angs_kern = atoi(temp.c_str()); ang_kern = new double[num_angs_kern];
	for (int i=0; i<num_angs_kern; i++) {infile >> temp; *(ang_kern+i) = atof(temp.c_str());}
	infile >> temp;
	infile >> temp; //artif images parameters END
	
	artif_im_param* artif_im_param_curr = new artif_im_param(im_dim, im_size, num_saltpepper_param_within, num_saltpepper_param, 
		saltpepper_param, num_Gauss_param_within, num_Gauss_param, Gauss_param, num_kern_param, kern_param, num_angs_kern, ang_kern);
	artif_im_param_read = artif_im_param_curr;

	infile >> save_artif_images; 
	infile >> temp; //texture model parameters BEGINNING
	infile >> create_synth_images; infile >> temp;
	infile >> text_mod_path; infile >> temp;
	infile >> search_str_text_mod; infile >> temp;
	infile >> text_mod_res_path; infile >> temp;
	int text_synth_dim=0; int* text_synth_size = NULL;	
	infile >> temp; text_synth_dim = atoi(temp.c_str()); text_synth_size = new int[text_synth_dim];
	for (int i=0; i<text_synth_dim; i++) {infile >> temp; *(text_synth_size+i) = atoi(temp.c_str());}
	infile >> temp;
	int* text_synth_kern_size = new int[text_synth_dim]; 
	for (int i=0; i<text_synth_dim; i++) {infile >> temp; *(text_synth_kern_size+i) = atoi(temp.c_str());}
	infile >> temp;
	int* text_synth_inset_size = new int[text_synth_dim]; 
	for (int i=0; i<text_synth_dim; i++) {infile >> temp; *(text_synth_inset_size+i) = atoi(temp.c_str());}
	infile >> temp;
	double text_synth_perc_val = 0;
	infile >> temp; text_synth_perc_val = atof(temp.c_str()); 
	infile >> temp;
	int* text_incr_decr = new int[4];
	for (int i=0; i<4; i++) {infile >> temp; *(text_incr_decr+i) = atoi(temp.c_str());}
	text_mod_param* text_mod_param_curr = new text_mod_param(text_synth_dim, text_synth_size, text_synth_kern_size,
		text_synth_inset_size, text_synth_perc_val, text_incr_decr);
	text_mod_param_read = text_mod_param_curr;
	infile >> temp;
	infile >> save_synth_images; infile >> temp; //texture model parameters END

	infile >> compute_Haralick_feat; infile >> temp; 
	infile >> Haralick_path; infile >> temp;
	infile >> search_str_images_Haralick; infile >> temp;
	infile >> res_Haralick_path; infile >> temp; 
	Haralick_smaller_wndw = new int[6];
	for (int i=0; i<6; i++) { infile >> temp; *(Haralick_smaller_wndw+i) = atoi(temp.c_str()); }
	infile >> temp;
	
	//Haralick parameters BEGINNING
	int offset_read=0; double ang_text_read=0; int num_bins_read=0; double max_val_read=0; double min_val_read=0;
	infile >> temp; offset_read = atoi(temp.c_str());
	infile >> temp; ang_text_read = atof(temp.c_str());
	infile >> temp; num_bins_read = atoi(temp.c_str());
	infile >> temp; max_val_read = atof(temp.c_str());
	infile >> temp; min_val_read = atof(temp.c_str()); 
	infile >> temp;
	int par2_yn_read=0; int num_offset_read=0; int* offset_list_read=NULL; double angoffset_read=0; double minang_read=0; 
		double maxang_read=0; int num_bins2_read=0; double max_val2_read=0; double min_val2_read=0; int tile_rows_read=0; 
		int tile_cols_read=0;
	infile >> temp; par2_yn_read = atoi(temp.c_str());
	infile >> temp; num_offset_read = atoi(temp.c_str());
	offset_list_read = new int[num_offset_read]; 
	for (int i=0; i<num_offset_read; i++) {infile >> temp; *(offset_list_read+i) = atoi(temp.c_str());}
	infile >> temp; angoffset_read = atof(temp.c_str());
	infile >> temp; minang_read = atof(temp.c_str());
	infile >> temp; maxang_read = atof(temp.c_str());
	infile >> temp; num_bins2_read = atoi(temp.c_str());
	infile >> temp; max_val2_read = atof(temp.c_str());
	infile >> temp; min_val2_read = atof(temp.c_str());
	infile >> temp; tile_rows_read = atoi(temp.c_str());
	infile >> temp; tile_cols_read = atoi(temp.c_str());
	infile >> temp; //Haralick parameters END

	Haralick_param* Haralick_param_curr = 
		new Haralick_param(offset_read, ang_text_read, num_bins_read, max_val_read, min_val_read, par2_yn_read, num_offset_read, 
		offset_list_read, angoffset_read, minang_read, maxang_read, num_bins2_read, max_val2_read, min_val2_read, tile_rows_read, 
		tile_cols_read);
	Haralick_param_read = Haralick_param_curr;
	infile >> save_Haralick_feat;
	infile.close();
}

//---------------------------------------------------------------------------

void search_filenames(list <string>& filename_list, string search_string, string namefile1_path, bool not_found)
{
	not_found = false;
	string namefile1 = namefile1_path + search_string;

	WIN32_FIND_DATAA fd;
	//const char* namefile1_ch = new char[50];
	//namefile1_ch = namefile1.c_str();
	//HANDLE h = FindFirstFile(namefile1_ch, &fd);
	HANDLE h = FindFirstFileA(namefile1.c_str(), &fd);
	if(h == INVALID_HANDLE_VALUE)
	{
		cout << "no files found!\n";
		not_found = true;
	}		
	while(1)
	{
		filename_list.push_back(fd.cFileName);
		if(FindNextFileA(h, &fd) == FALSE)
			break;
	}
	int filename_list_sz = filename_list.size();
}

//---------------------------------------------------------------------------

void parse_txt_by_space(string namefile, string*& word_list, int& num_words)
{
    const char* namefile_ch = new char[50];
    namefile_ch = namefile.c_str();

    ifstream infile;
    infile.open(namefile_ch);

    if (!infile)
    {
        cout << "File not found\n";
    }

    if ((infile)&&(!infile.is_open()))
    {
        cout << "File not open\n";
    }

    string temp;
	int i=0;
	infile >> temp;
    while (!infile.eof())
    {        
		*(word_list+i) = temp;
		i++;
		infile >> temp;
    }
	num_words = i;
}

//---------------------------------------------------------------------------

void parse_txt_by_space(string namefile, double*& word_list, int& num_words)
{
    const char* namefile_ch = new char[50];
    namefile_ch = namefile.c_str();

    ifstream infile;
    infile.open(namefile_ch);

    if (!infile)
    {
        cout << "File not found\n";
    }

    if ((infile)&&(!infile.is_open()))
    {
        cout << "File not open\n";
    }

    double temp;
	int i=0;
	infile >> temp;
    while (!infile.eof())
    {        
		*(word_list+i) = temp;
		i++;
		infile >> temp;
    }
	num_words = i;
}

//---------------------------------------------------------------------------
// AntoniosHaralickv001.cpp : Defines the entry point for the console application.
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/opencv.hpp>
#include <iostream>
#include <fstream>
#include<list>
#include<direct.h>
#include <ctime>
#include "IO_functions.h"
#include "my_functions.h"
#include "parameter_classes.h"

using namespace cv;
using namespace std;

int main(int argc, const char* argv[])
{
	//parse config_text_proj.txt
	string namefile_config(argv[1], 0, 100);
	string read_images, images_path, search_str_images, process_read_images, visualize_read_images, create_artif_images,
		artif_images_path, save_artif_images, create_synth_images, text_mod_path, search_str_text_mod, text_mod_res_path,
		save_synth_images, compute_Haralick_feat, Haralick_path, search_str_images_Haralick, res_Haralick_path, save_Haralick_feat;
	artif_im_param* artif_im_param_read = NULL; text_mod_param* text_mod_param_read = NULL; Haralick_param* Haralick_param_read = NULL;
	int* Haralick_smaller_wndw = NULL;
	parse_config(namefile_config, read_images, images_path, search_str_images, process_read_images, visualize_read_images,
		create_artif_images, artif_images_path, artif_im_param_read, save_artif_images, create_synth_images, text_mod_path,
		search_str_text_mod, text_mod_res_path, text_mod_param_read, save_synth_images, compute_Haralick_feat, Haralick_path,
		search_str_images_Haralick, res_Haralick_path, Haralick_smaller_wndw, Haralick_param_read, save_Haralick_feat);

	int im_dim = 0; int* im_size = NULL; int num_saltpepper_param_within = 0; int num_saltpepper_param = 0; int** saltpepper_param = NULL;
	int num_Gauss_param_within = 0; int num_Gauss_param = 0; double** Gauss_param = NULL; int num_kern_param = 0; int** kern_param = NULL;
	int num_angs_kern = 0; double* ang_kern = NULL;
	artif_im_param_read->get_im_dim(im_dim); artif_im_param_read->get_im_size(im_size);
	artif_im_param_read->get_num_saltpepper_param_within(num_saltpepper_param_within);
	artif_im_param_read->get_num_saltpepper_param(num_saltpepper_param); artif_im_param_read->get_saltpepper_param(saltpepper_param);
	artif_im_param_read->get_num_Gauss_param_within(num_Gauss_param_within);
	artif_im_param_read->get_num_Gauss_param(num_Gauss_param); artif_im_param_read->get_Gauss_param(Gauss_param);
	artif_im_param_read->get_num_kern_param(num_kern_param); artif_im_param_read->get_kern_param(kern_param);
	artif_im_param_read->get_num_angs_kern(num_angs_kern); artif_im_param_read->get_ang_kern(ang_kern);

	/*cout << im_dim << endl; for (int i=0; i<im_dim; i++) {cout << *(im_size+i) << endl;}
	cout << num_saltpepper_param_within << endl;
	cout << num_saltpepper_param << endl;
	for (int i=0; i<num_saltpepper_param; i++) { for (int j=0; j<num_saltpepper_param_within; j++)
	{ cout << *(*(saltpepper_param+i)+j) << endl; } }
	cout << num_Gauss_param_within << endl;
	cout << num_Gauss_param << endl;
	for (int i=0; i<num_Gauss_param; i++) { for (int j=0; j<num_Gauss_param_within; j++) { cout << *(*(Gauss_param+i)+j) << endl; } }
	cout << num_kern_param << endl;
	for (int i=0; i<num_kern_param; i++) { for (int j=0; j<4; j++) { cout << *(*(kern_param+i)+j) << endl; } }
	cout << num_angs_kern << endl; for (int i=0; i<num_angs_kern; i++) {cout << *(ang_kern+i) << endl;}*/

	int synth_im_dim = 0; int* synth_im_sz = NULL; int* synth_kernel_size = NULL; int* synth_inset_size = NULL; double perc_val = 0;
	int* synth_incr_decr = new int[4];
	text_mod_param_read->get_im_dim(synth_im_dim); text_mod_param_read->get_im_size(synth_im_sz);
	text_mod_param_read->get_kernel_size(synth_kernel_size); text_mod_param_read->get_inset_size(synth_inset_size);
	text_mod_param_read->get_perc_val(perc_val); text_mod_param_read->get_incr_decr(synth_incr_decr);

	/*cout << synth_im_dim << endl; for (int i=0; i<synth_im_dim; i++) {cout << *(synth_im_sz+i) << endl;}
	for (int i=0; i<synth_im_dim; i++) {cout << *(synth_kernel_size+i) << endl;}
	for (int i=0; i<synth_im_dim; i++) {cout << *(synth_inset_size+i) << endl;}
	cout << perc_val << endl;
	for (int i=0; i<4; i++) {cout << *(synth_incr_decr+i) << endl;}*/

	int offset = 0; double ang_text = 0; int num_bins = 0; double max_val = 0; double min_val = 0;
	Haralick_param_read->get_offset(offset);
	Haralick_param_read->get_ang_text(ang_text); Haralick_param_read->get_num_bins(num_bins);
	Haralick_param_read->get_max_val(max_val); Haralick_param_read->get_min_val(min_val);

	/*for (int i=0; i<6; i++) { cout << *(Haralick_smaller_wndw+i) << endl; }
	cout << offset << endl; cout << ang_text << endl; cout << num_bins << endl; cout << max_val << endl; cout << min_val << endl;*/

	int par2_yn = 0; int num_offset = 0; int* offset_list = NULL; double angoffset_text = 0; double minang_text = 0;
	double maxang_text = 0; int num_bins2 = 0; double max_val2 = 0; double min_val2 = 0; int tile_row_sz = 0; int tile_col_sz = 0;
	Haralick_param_read->get_par2_yn(par2_yn); Haralick_param_read->get_num_offset(num_offset);
	Haralick_param_read->get_offset_list(offset_list); Haralick_param_read->get_angoffset(angoffset_text);
	Haralick_param_read->get_minang(minang_text); Haralick_param_read->get_maxang(maxang_text);
	Haralick_param_read->get_num_bins2(num_bins2); Haralick_param_read->get_max_val2(max_val2);
	Haralick_param_read->get_min_val2(min_val2); Haralick_param_read->get_tile_rows(tile_row_sz);
	Haralick_param_read->get_tile_cols(tile_col_sz);

	/*cout << par2_yn << endl;
	for (int i=0; i<num_offset; i++) { cout << *(offset_list+i) << endl; }
	cout << angoffset_text << endl; cout << minang_text << endl; cout << maxang_text << endl;
	cout << num_bins2 << endl; cout << max_val2 << endl; cout << min_val2 << endl;
	cout << tile_row_sz << endl; cout << tile_col_sz << endl;*/

	// read images
	bool not_found = false;
	list <Mat*> im_list;
	list <Mat*> im_proc_list;
	list <string> filename_list;
	int read_images_yn = atoi(read_images.c_str());
	if (read_images_yn == 1)
	{
		search_filenames(filename_list, search_str_images, images_path, not_found);
		if (not_found) return -1;

		//list <Mat*> im_list;
		list<string>::iterator ifilename_list = filename_list.begin();
		while (ifilename_list != filename_list.end())
		{
			Mat* im = new Mat;
			string full_name = images_path + (*ifilename_list);
			*im = imread(full_name, -1);
			if (im->empty())
			{
				cout << "Cannot load image!";
				return -1;
			}
			im_list.push_back(im);
			ifilename_list++;
		}

		int process_read_images_yn = atoi(process_read_images.c_str());
		if (process_read_images_yn == 1)
		{

			/*Mat im_tmp = (Mat_<double>(3,3)<<0,1,0,0,0,0,0,0,0);
			imwrite("C:/Users/toniocar/Desktop/Texture_material/texture_VS_project/Input_images/cell_images/im_tmp.tif", im_tmp);
			double angle_rot = 23;
			Point center_rot(1,1);
			double scale_rot = 1;
			Mat rot_mat = getRotationMatrix2D(center_rot, angle_rot, scale_rot);
			imwrite("C:/Users/toniocar/Desktop/Texture_material/texture_VS_project/Input_images/cell_images/rot_mat.tif", rot_mat);
			Mat im_tmp_rot;
			warpAffine(im_tmp, im_tmp_rot, rot_mat, im_tmp.size(), INTER_CUBIC);
			imwrite("C:/Users/toniocar/Desktop/Texture_material/texture_VS_project/Input_images/cell_images/im_tmp_rot.tif", im_tmp_rot*255);
			Mat im_tmp_proc = (Mat_<double>(3,3)<<3,5,8,9,32,1,4,5,7);
			multiply(im_tmp_proc, im_tmp_rot, im_tmp_proc);
			imwrite("C:/Users/toniocar/Desktop/Texture_material/texture_VS_project/Input_images/cell_images/im_tmp_proc.tif", im_tmp_proc);*/

			list<Mat*>::iterator iim_list = im_list.begin();
			while (iim_list != im_list.end())
			{
				Mat *im_proc = new Mat;
				GaussianBlur(*(*iim_list), *im_proc, Size(35, 35), 2.);
				waitKey(0);
				im_proc_list.push_back(im_proc);
				iim_list++;
			}
		}

		int visualize_read_images_yn = atoi(visualize_read_images.c_str());
		if (visualize_read_images_yn == 1)
		{
			list<Mat*>::iterator iim_list = im_list.begin();
			list<Mat*>::iterator iim_proc_list = im_proc_list.begin();
			while (iim_list != im_list.end())
			{
				imshow("Image", **iim_list);
				waitKey(0);
				imshow("Image", **iim_proc_list);
				waitKey(0);
				iim_proc_list++;
				iim_list++;
			}
		}
	}


	list <Mat*> im_init_list;
	list <Mat*> kernel_list;
	list <Mat*> im_artif_list;


	// create artificial images

	int create_artif_images_yn = atoi(create_artif_images.c_str());
	if (create_artif_images_yn == 1)
	{
		const char* artif_images_path_ch = new char[50];
		artif_images_path_ch = artif_images_path.c_str();
		mkdir(artif_images_path_ch);

		// Build initial images
		cout << "Initial noise images to build: " << (num_saltpepper_param + num_Gauss_param) << endl << endl;
		Mat* im_init = NULL;
		if (im_dim == 2) {
			for (int i = 0; i < num_saltpepper_param; i++) {
				im_init = new Mat((*im_size), (*(im_size + 1)), CV_16U);
				randu(*im_init, *(*(saltpepper_param + i)), *(*(saltpepper_param + i) + 1)); // SALT AND PEPPER NOISE
				im_init_list.push_back(im_init);
			}
			for (int i = 0; i < num_Gauss_param; i++) {
				im_init = new Mat((*im_size), (*(im_size + 1)), CV_16U);
				randn(*im_init, *(*(Gauss_param + i)), *(*(Gauss_param + i) + 1)); // GAUSS NOISE
				im_init_list.push_back(im_init);
			}
		}
		cout << "Built initial noise images!" << endl << endl;

		//Build kernels
		cout << "Building initial kernels" << endl << endl;
		for (int k = 0; k<num_kern_param; k++)
		{
			int size_kernel = *(*(kern_param + k) + 0);
			for (int j = 0; j<*(*(kern_param + k) + 2); j++)
			{
				int num_diag = 1 + j*(*(*(kern_param + k) + 1));
				Mat* kernel = new Mat(size_kernel, size_kernel, CV_32F, 0.);
				//Mat kernel = Mat::zeros(size_kernel,size_kernel,CV_32F);
				//Mat temp_mat1(1,1,CV_32S);
				Mat* temp_mat1 = new Mat(1, 1, CV_32S);
				//temp_mat1.at<int>(0,0) = 2;
				(*temp_mat1).at<int>(0, 0) = 2;
				for (int i = 0; i<size_kernel; i++)
				{
					for (int j = 0; j<size_kernel; j++)
					{
						int i_num_diag = 0;
						while (i_num_diag<num_diag)
						{
							if (abs(i - j) == i_num_diag)
							{
								//Mat temp_mat2(1,1,CV_32S);
								Mat* temp_mat2 = new Mat(1, 1, CV_32S);
								pow(*temp_mat1, i_num_diag, *temp_mat2);
								kernel->at<float>(i, j) = (float)(*(*(kern_param + k) + 3)) / (*temp_mat2).at<int>(0, 0);
								delete temp_mat2;
								break;
							}
							i_num_diag++;
						}
					}
				}
				delete temp_mat1;
				kernel_list.push_back(kernel);

			}
		}
		cout << "Built " << kernel_list.size() << " initial kernel images!" << endl << endl;

		//Rotate built kernels
		cout << "Rotating initial kernels for a total of " << kernel_list.size()*num_angs_kern << " kernel images!" << endl << endl;
		list<Mat*>::iterator ikernel_list = kernel_list.begin();
		list<Scalar> list_mysums;
		int count_tmp = 0;
		while (ikernel_list != kernel_list.end())
		{
			Mat* kernel = NULL;
			kernel = (*ikernel_list);
			ikernel_list = kernel_list.erase(ikernel_list);
			for (int i = 0; i<num_angs_kern; i++)
			{
				double angle_rot = *(ang_kern + i);
				Mat* rot_mat = new Mat(2, 3, CV_32F);
				int size_kernel1 = kernel->rows;
				int size_kernel2 = kernel->cols;
				Point center_rot((size_kernel1 - 1) / 2, (size_kernel2 - 1) / 2);
				double scale_rot = 1;
				*rot_mat = getRotationMatrix2D(center_rot, angle_rot, scale_rot);
				Mat* kernel_rot = new Mat(kernel->rows, kernel->cols, kernel->type(), 0.);
				warpAffine(*kernel, *kernel_rot, *rot_mat, kernel->size(), INTER_CUBIC);
				Scalar mysum_rot = sum(*kernel_rot);
				*(kernel_rot) = *(kernel_rot) / mysum_rot[0];
				list_mysums.push_back(mysum_rot);
				kernel_list.insert(ikernel_list, kernel_rot);
				count_tmp++;
				if (count_tmp % 20 == 0) {
					cout << count_tmp << endl;
				}
			}
		}
		cout << endl;
		cout << "Rotated initial kernels images!" << endl << endl;

		//Filter initial images
		cout << "Filtering images for a total of " << kernel_list.size()*im_init_list.size() << " images!" << endl << endl;
		list<Mat*>::iterator iim_init_list = im_init_list.begin();
		count_tmp = 0;
		while (iim_init_list != im_init_list.end())
		{
			list<Mat*>::iterator ikernel_list = kernel_list.begin();
			while (ikernel_list != kernel_list.end())
			{
				Mat* im_artif_curr = new Mat(im_init->size(), CV_16U);
				filter2D(*(*iim_init_list), *im_artif_curr, -1, *(*ikernel_list));
				im_artif_list.push_back(im_artif_curr);
				ikernel_list++;
				count_tmp++;
				if (count_tmp % 20 == 0) {
					cout << count_tmp << endl;
				}
			}
			iim_init_list++;
		}
		cout << endl;
		cout << "Filtered kernel images!" << endl << endl;
		cout << "Denormalizing " << kernel_list.size() << " kernel images!" << endl << endl;
		list<Scalar>::iterator ilist_mysums = list_mysums.begin();
		ikernel_list = kernel_list.begin();
		count_tmp = 0;
		while (ilist_mysums != list_mysums.end())
		{
			*(*ikernel_list) = *(*ikernel_list) * (*ilist_mysums)[0];
			ilist_mysums++; ikernel_list++;
			count_tmp++;
			if (count_tmp % 20 == 0) {
				cout << count_tmp << endl;
			}
		}
		cout << endl;
		cout << "Denormalized " << kernel_list.size() << " kernel images!" << endl << endl;

		// save artifical images, kernel images (not normalized) and initial noise images
		int save_artif_images_yn = atoi(save_artif_images.c_str());
		if (save_artif_images_yn == 1)
		{

			list<Mat*>::iterator iim_init_list = im_init_list.begin();
			int count = 1;
			while (iim_init_list != im_init_list.end())
			{
				string namefile_curr = "Init_S&P"; int curr_index = count;
				int num_within = num_saltpepper_param_within;
				bool isint = true; bool isdouble = false; int** int_param = saltpepper_param; double** double_param = NULL;
				if (count > num_saltpepper_param) {
					namefile_curr = "Init_Gauss"; curr_index = count - num_saltpepper_param;
					num_within = num_Gauss_param_within;
					isint = false; isdouble = true; double_param = Gauss_param; int_param = NULL;
				} // as we add noise creators, elseif cycles should be added the same way, in the proper order. 
				// If new types appear, change code accordingly
				for (int i = 0; i<num_within; i++) {
					namefile_curr = namefile_curr + "_";
					int temp_int; double temp_double;
					if (isint) {
						int temp_int = *(*(int_param + curr_index - 1) + i);
						ostringstream tmp_ostring;
						tmp_ostring << temp_int; string tmp_string = tmp_ostring.str(); namefile_curr = namefile_curr + tmp_string;
					}
					if (isdouble) {
						double temp_double = *(*(double_param + curr_index - 1) + i);
						ostringstream tmp_ostring;
						tmp_ostring << temp_double; string tmp_string = tmp_ostring.str(); namefile_curr = namefile_curr + tmp_string;
					}
				}
				namefile_curr = namefile_curr + ".tiff";
				imwrite(artif_images_path + namefile_curr, *(*iim_init_list));
				iim_init_list++;
				count++;
			}

			list<Mat*>::iterator ikernel_list = kernel_list.begin();
			for (int k = 0; k<num_kern_param; k++)
			{

				if (ikernel_list == kernel_list.end())
				{
					cout << "kernel list files are less than expected(1)!!!\n"; break;
				}
				int size_kernel = *(*(kern_param + k) + 0); ostringstream tmp_ostring;
				tmp_ostring << size_kernel; string size_kernel_string = tmp_ostring.str();
				int max_val = (*(*(kern_param + k) + 3)); ostringstream tmp_ostring2;
				tmp_ostring2 << max_val; string max_val_string = tmp_ostring2.str();
				for (int j = 0; j<*(*(kern_param + k) + 2); j++)
				{
					if (ikernel_list == kernel_list.end())
					{
						cout << "kernel list files are less than expected(2)!!!\n"; break;
					}
					int num_diag = 1 + j*(*(*(kern_param + k) + 1)); ostringstream tmp_ostring;
					tmp_ostring << num_diag; string num_diag_string = tmp_ostring.str();
					string namefile_curr = "Kernel"; namefile_curr = namefile_curr + "_max" + max_val_string;
					namefile_curr = namefile_curr + "_sz" + size_kernel_string + "_dg" + num_diag_string;
					for (int i = 0; i<num_angs_kern; i++)
					{
						double angle_rot = *(ang_kern + i); ostringstream tmp_ostring;
						tmp_ostring << angle_rot; string angle_rot_string = tmp_ostring.str();
						tmp_ostring << i; string i_string = tmp_ostring.str();
						if (i<10) { i_string = "0" + i_string; } //CHANGE THIS IF num_angs_kern>100!!! 
						string namefile_curr_fin = namefile_curr + "_rot" + i_string + "_" + angle_rot_string;
						namefile_curr_fin = namefile_curr_fin + ".tiff";
						imwrite(artif_images_path + namefile_curr_fin, *(*ikernel_list));
						ikernel_list++;
					}

				}
			}


			iim_init_list = im_init_list.begin();
			list<Mat*>::iterator iim_artif_list = im_artif_list.begin();
			count = 1;
			while (iim_init_list != im_init_list.end())
			{
				string namefile_curr = "S&P"; int curr_index = count;
				int num_within = num_saltpepper_param_within;
				bool isint = true; bool isdouble = false; int** int_param = saltpepper_param; double** double_param = NULL;
				if (count > num_saltpepper_param) {
					namefile_curr = "Gauss"; curr_index = count - num_saltpepper_param;
					num_within = num_Gauss_param_within;
					isint = false; isdouble = true; double_param = Gauss_param; int_param = NULL;
				} // as we add noise creators, elseif cycles should be added the same way, in the proper order. 
				// If new types appear, change code accordingly
				for (int i = 0; i<num_within; i++) {
					namefile_curr = namefile_curr + "_";
					int temp_int; double temp_double;
					if (isint) {
						int temp_int = *(*(int_param + curr_index - 1) + i);
						ostringstream tmp_ostring;
						tmp_ostring << temp_int; string tmp_string = tmp_ostring.str(); namefile_curr = namefile_curr + tmp_string;
					}
					if (isdouble) {
						double temp_double = *(*(double_param + curr_index - 1) + i);
						ostringstream tmp_ostring;
						tmp_ostring << temp_double; string tmp_string = tmp_ostring.str(); namefile_curr = namefile_curr + tmp_string;
					}
				}

				for (int k = 0; k<num_kern_param; k++)
				{

					if (iim_artif_list == im_artif_list.end())
					{
						cout << "artificial list files are less than expected(3)!!!\n"; break;
					}
					int size_kernel = *(*(kern_param + k) + 0); ostringstream tmp_ostring;
					tmp_ostring << size_kernel; string size_kernel_string = tmp_ostring.str();
					int max_val = (*(*(kern_param + k) + 3)); ostringstream tmp_ostring2;
					tmp_ostring2 << max_val; string max_val_string = tmp_ostring2.str();
					for (int j = 0; j<*(*(kern_param + k) + 2); j++)
					{
						if (iim_artif_list == im_artif_list.end())
						{
							cout << "artificial list files are less than expected(4)!!!\n"; break;
						}
						int num_diag = 1 + j*(*(*(kern_param + k) + 1)); ostringstream tmp_ostring;
						tmp_ostring << num_diag; string num_diag_string = tmp_ostring.str();
						string namefile_curr_fin1 = namefile_curr + "_krn_max" + max_val_string;
						namefile_curr_fin1 = namefile_curr_fin1 + "_sz" + size_kernel_string + "_dg" + num_diag_string;
						for (int i = 0; i<num_angs_kern; i++)
						{
							double angle_rot = *(ang_kern + i); ostringstream tmp_ostring;
							tmp_ostring << angle_rot; string angle_rot_string = tmp_ostring.str();
							tmp_ostring << i; string i_string = tmp_ostring.str();
							if (i<10) { i_string = "0" + i_string; } //CHANGE THIS IF num_angs_kern>100!!!
							string namefile_curr_fin2 = namefile_curr_fin1 + "_rot" + i_string + "_" + angle_rot_string;
							namefile_curr_fin2 = namefile_curr_fin2 + ".tiff";
							imwrite(artif_images_path + namefile_curr_fin2, *(*iim_artif_list));
							iim_artif_list++;
						}

					}
				}

				iim_init_list++;
				count++;
			}

		}
	}


	int create_synth_images_yn = atoi(create_synth_images.c_str());
	int save_synth_images_yn = atoi(save_synth_images.c_str());
	if (create_synth_images_yn == 1)
	{
		// read text models
		bool not_found = false;
		list <Mat*> text_mod_list;
		list <string> filename_list_tm;
		search_filenames(filename_list_tm, search_str_text_mod, text_mod_path, not_found);
		if (not_found) return -1;

		list<string>::iterator ifilename_list_tm = filename_list_tm.begin();
		while (ifilename_list_tm != filename_list_tm.end())
		{
			Mat* text_mod = new Mat;
			string full_name = text_mod_path + (*ifilename_list_tm);
			*text_mod = imread(full_name, -1);
			if (text_mod->empty())
			{
				cout << "Cannot load image!";
				return -1;
			}
			text_mod_list.push_back(text_mod);
			ifilename_list_tm++;
		}

		// Booleans for transversing direction		
		bool colpos1_incr = true; bool colpos2_incr = true; bool rowpos1_incr = true; bool rowpos2_incr = true;
		if (*synth_incr_decr != 0) colpos1_incr = false; if (*(synth_incr_decr + 1) != 0) colpos2_incr = false;
		if (*(synth_incr_decr + 2) != 0) rowpos1_incr = false; if (*(synth_incr_decr + 3) != 0) rowpos2_incr = false;
		string s_tmp6 = ""; if (!colpos1_incr) s_tmp6 = s_tmp6 + "l"; if (!colpos2_incr) s_tmp6 = s_tmp6 + "r";
		if (!rowpos1_incr) s_tmp6 = s_tmp6 + "t"; if (!rowpos2_incr) s_tmp6 = s_tmp6 + "b";

		list <Mat*> synth_im_list;
		list<Mat*>::iterator itext_mod_list = text_mod_list.begin();
		ifilename_list_tm = filename_list_tm.begin();
		while (itext_mod_list != text_mod_list.end())
		{
			int num_row_mod = (*itext_mod_list)->rows; int num_col_mod = (*itext_mod_list)->cols;
			Mat* synth_im_curr = new Mat((*synth_im_sz), (*(synth_im_sz + 1)), CV_16U, 0.);
			int col_pos_mod = *(synth_im_sz + 1) / 2 - num_col_mod / 2; int row_pos_mod = *(synth_im_sz) / 2 - num_row_mod / 2;
			Mat roi_mod = (*synth_im_curr)(Rect(col_pos_mod, row_pos_mod, num_col_mod, num_row_mod));
			(*(*itext_mod_list)).copyTo(roi_mod);

			int num_row_roi = *synth_kernel_size; int num_col_roi = *(synth_kernel_size + 1);
			int mod_inset_rows = *synth_inset_size; int mod_inset_cols = *(synth_inset_size + 1);
			double perc_smaller_text_mod = perc_val;
			string path_tmp = text_mod_res_path;

			// Define padding based on ROI size and texture synthesis algorithm			
			int pad_top = (num_row_roi - 1) / 2; int pad_bottom = (num_row_roi - 1) / 2;
			int pad_left = (num_col_roi - 1) / 2; int pad_right = (num_col_roi - 1) / 2;

			const char* path_tmp_ch = new char[50];
			string tempo = text_mod_path + path_tmp;
			path_tmp_ch = tempo.c_str();
			mkdir(path_tmp_ch);

			//Pad synth image			
			Mat* synth_im_curr_pad = new Mat();
			copyMakeBorder(*synth_im_curr, *synth_im_curr_pad, pad_top, pad_bottom, pad_left, pad_right, BORDER_DEFAULT);

			if (save_synth_images_yn) {
				stringstream temponame; temponame << num_row_roi; string s_tmp1 = temponame.str(); temponame.str("");
				temponame << num_col_roi; string s_tmp2 = temponame.str();  temponame.str("");
				temponame << mod_inset_rows; string s_tmp3 = temponame.str();  temponame.str("");
				temponame << mod_inset_cols; string s_tmp4 = temponame.str();  temponame.str("");
				temponame << perc_smaller_text_mod; string s_tmp5 = temponame.str();  temponame.str("");
				string filename_temponame = "pad_init_sz" + s_tmp1 + "_" + s_tmp2 + "-" + s_tmp3 + "_" + s_tmp4 + "_pc" + s_tmp5 +
					"_Olrtb_D" + s_tmp6 + "_" + *(ifilename_list_tm);
				//imwrite(text_mod_path+path_tmp+filename_temponame, *synth_im_curr_pad);
				filename_temponame = "init_sz" + s_tmp1 + "_" + s_tmp2 + "-" + s_tmp3 + "_" + s_tmp4 + "_pc" + s_tmp5 + "_Olrtb_D" + s_tmp6 + "_"
					+ *(ifilename_list_tm);
				imwrite(text_mod_path + path_tmp + filename_temponame, *synth_im_curr);

				////imwrite(text_mod_path+path_tmp+"pad_init_sz47_47-84_128_pc0_Olrtb_D_TestTextureBars2.tiff", *synth_im_curr_pad);
				//imwrite(text_mod_path+path_tmp+"init_sz47_47-84_128_pc0_Olrtb_D_TestTextureBars2.tiff", *synth_im_curr);					
			}

			int num_row_synth_im_curr_pad = synth_im_curr_pad->rows;
			int num_col_synth_im_curr_pad = synth_im_curr_pad->cols;

			// process synth_im_curr_pad			
			int col_pos_mod_pad = col_pos_mod + pad_left; int row_pos_mod_pad = row_pos_mod + pad_top;

			/*Mat roi_synth_im_curr_pad1 = (*synth_im_curr_pad)(Rect(synth_im_curr_pad->cols/2, synth_im_curr_pad->rows/2, 4, 4));
			Mat roi_synth_im_curr_pad2 = (*synth_im_curr_pad)(Rect(synth_im_curr_pad->cols/2+5, synth_im_curr_pad->rows/2+10, 4, 4));
			Mat roi_result = (roi_synth_im_curr_pad1+roi_synth_im_curr_pad2);
			divide(roi_result,roi_result, roi_result);
			imwrite(text_mod_path+"prova1.tiff", roi_synth_im_curr_pad1);
			imwrite(text_mod_path+"prova2.tiff", roi_synth_im_curr_pad2);
			imwrite(text_mod_path+"prova3.tiff", roi_result);*/

			// initial positions of ROI centers for col and row transverse
			int* pos_col_transv = new int[4]; int* pos_row_transv = new int[4];

			//*(pos_col_transv+0) = col_pos_mod_pad-1+(num_col_roi-1)/2; //col limit left
			//*(pos_col_transv+1) = col_pos_mod_pad+num_col_mod-(num_col_roi-1)/2; //col limit right
			//*(pos_col_transv+2) = row_pos_mod_pad; //row initial position
			//*(pos_col_transv+3) = row_pos_mod_pad+num_row_mod-1; //row final position
			//*(pos_row_transv+0) = col_pos_mod_pad; //col initial position
			//*(pos_row_transv+1) = col_pos_mod_pad+num_col_mod-1; //col final position
			//*(pos_row_transv+2) = row_pos_mod_pad-1+(num_row_roi-1)/2; //row limit top
			//*(pos_row_transv+3) = row_pos_mod_pad+num_row_mod-(num_row_roi-1)/2; //row limit bottom

			*(pos_col_transv + 0) = col_pos_mod_pad - 1; //col limit left
			*(pos_col_transv + 1) = col_pos_mod_pad + num_col_mod; //col limit right
			*(pos_col_transv + 2) = row_pos_mod_pad; //row initial position
			*(pos_col_transv + 3) = row_pos_mod_pad + num_row_mod - 1; //row final position
			*(pos_row_transv + 0) = col_pos_mod_pad; //col initial position
			*(pos_row_transv + 1) = col_pos_mod_pad + num_col_mod - 1; //col final position
			*(pos_row_transv + 2) = row_pos_mod_pad - 1; //row limit top
			*(pos_row_transv + 3) = row_pos_mod_pad + num_row_mod; //row limit bottom

			// Booleans for limit reach
			bool colpos1_lim = false; bool colpos2_lim = false; bool rowpos1_lim = false; bool rowpos2_lim = false;

			//these four if statements should be copied in process_mod_border function
			/*if (*(pos_col_transv+0)-(num_col_roi-1)/2<pad_left) {colpos1_lim=true;}
			if (*(pos_col_transv+1)+(num_col_roi-1)/2>num_col_synth_im_curr_pad-1-pad_right) {colpos2_lim=true;}
			if (*(pos_row_transv+2)-(num_row_roi-1)/2<pad_top) {rowpos1_lim=true;}
			if (*(pos_row_transv+3)+(num_row_roi-1)/2>num_row_synth_im_curr_pad-1-pad_bottom) {rowpos2_lim=true;}*/
			if (*(pos_col_transv + 0)<pad_left) { colpos1_lim = true; }
			if (*(pos_col_transv + 1)>num_col_synth_im_curr_pad - 1 - pad_right) { colpos2_lim = true; }
			if (*(pos_row_transv + 2)<pad_top) { rowpos1_lim = true; }
			if (*(pos_row_transv + 3)>num_row_synth_im_curr_pad - 1 - pad_bottom) { rowpos2_lim = true; }

			int count_while = 0;
			srand((unsigned)time(0));
			while (!colpos1_lim || !colpos2_lim || !rowpos1_lim || !rowpos2_lim) {
				process_mod_border(colpos1_lim, colpos2_lim, rowpos1_lim, rowpos2_lim, pos_col_transv, pos_row_transv, pad_left,
					pad_right, pad_top, pad_bottom, synth_im_curr_pad, num_col_roi, num_row_roi, mod_inset_rows, mod_inset_cols,
					(*itext_mod_list), num_col_synth_im_curr_pad, num_row_synth_im_curr_pad, perc_smaller_text_mod, colpos1_incr,
					colpos2_incr, rowpos1_incr, rowpos2_incr, count_while);
			}


			//Unpad synth image
			Mat roi_unpad = (*synth_im_curr_pad)(Rect(pad_left, pad_top, (*synth_im_curr).cols, (*synth_im_curr).rows));
			roi_unpad.copyTo(*synth_im_curr);

			if (save_synth_images_yn) {
				stringstream temponame; temponame << num_row_roi; string s_tmp1 = temponame.str(); temponame.str("");
				temponame << num_col_roi; string s_tmp2 = temponame.str(); temponame.str("");
				temponame << mod_inset_rows; string s_tmp3 = temponame.str(); temponame.str("");
				temponame << mod_inset_cols; string s_tmp4 = temponame.str(); temponame.str("");
				temponame << perc_smaller_text_mod; string s_tmp5 = temponame.str(); temponame.str("");
				string filename_temponame = "pad_fin_sz" + s_tmp1 + "_" + s_tmp2 + "-" + s_tmp3 + "_" + s_tmp4 + "_pc" + s_tmp5 +
					"_Olrtb_D" + s_tmp6 + "_" + *(ifilename_list_tm);
				//imwrite(text_mod_path+path_tmp+filename_temponame, *synth_im_curr_pad);
				filename_temponame = "fin_sz" + s_tmp1 + "_" + s_tmp2 + "-" + s_tmp3 + "_" + s_tmp4 + "_pc" + s_tmp5 + "_Olrtb_D" + s_tmp6 + "_"
					+ *(ifilename_list_tm);
				imwrite(text_mod_path + path_tmp + filename_temponame, *synth_im_curr);

				////imwrite(text_mod_path+path_tmp+"pad_fin_sz47_47-84_128_pc0_Olrtb_D_TestTextureBars2.tiff", *synth_im_curr_pad);
				//imwrite(text_mod_path+path_tmp+"fin_sz47_47-84_128_pc0_Olrtb_D_TestTextureBars2.tiff", *synth_im_curr);	
			}

			synth_im_list.push_back(synth_im_curr);
			itext_mod_list++;
			ifilename_list_tm++;
		}

	}

	int compute_Haralick_feat_yn = atoi(compute_Haralick_feat.c_str());
	if (compute_Haralick_feat_yn == 1)
	{
		srand((unsigned)time(0));
		bool not_found = false;
		list <Mat*> im_list;
		list <string> filename_list;

		search_filenames(filename_list, search_str_images_Haralick, Haralick_path, not_found);
		filename_list.sort();
		if (not_found) return -1;

		list<string>::iterator ifilename_list = filename_list.begin();
		while (ifilename_list != filename_list.end())
		{
			Mat* im = new Mat;
			string full_name = Haralick_path + (*ifilename_list);
			*im = imread(full_name, -1);
			if (im->empty())
			{
				cout << "Cannot load image!"; return -1;
			}
			im_list.push_back(im);
			ifilename_list++;
		}

		// for par2_yn==0
		double* contrast_vect = new double[im_list.size()]; double* energy_vect = new double[im_list.size()];
		double* homogeneity_vect = new double[im_list.size()]; double* correlation_vect = new double[im_list.size()];

		// for par2_yn==1
		int* Haralick_answer_fin = new int[im_list.size()]; double* Haralick_margin_fin = new double[im_list.size()];

		// for par2_yn==2
		int** Haralick_answers_fin = new int*[im_list.size()];
		int* size_Haralick_answers_fin = new int[im_list.size()];

		// for par2_yn==3
		int* num_row_tiles_Haralick_fin = new int[im_list.size()]; int* num_col_tiles_Haralick_fin = new int[im_list.size()];
		//          angle output
		int** Haralick_answer_ang_fin = new int*[im_list.size()]; double** Haralick_margin_ang_fin = new double*[im_list.size()];
		//          offset output
		int*** Haralick_answers_offset_fin = new int**[im_list.size()];



		list <Mat*> co_occurrence_mat_list;
		list<Mat*>::iterator iim_list = im_list.begin();
		ifilename_list = filename_list.begin();
		int count_Haralick = 0;
		while (iim_list != im_list.end())
		{
			Mat* im_curr = new Mat();
			if (*(Haralick_smaller_wndw) == 1) {
				if ((*iim_list)->rows<*(Haralick_smaller_wndw + 1) || (*iim_list)->cols<*(Haralick_smaller_wndw + 2)) {
					im_curr = (*iim_list);
				}
				else {
					if (*(Haralick_smaller_wndw + 3) == 0) {
						(*im_curr) = (*(*iim_list))(Rect(*(Haralick_smaller_wndw + 5), *(Haralick_smaller_wndw + 4),
							*(Haralick_smaller_wndw + 2), *(Haralick_smaller_wndw + 1)));
					}
					else {
						int pos_r = ((*iim_list)->rows - *(Haralick_smaller_wndw + 1) - 1)*rand() / RAND_MAX;
						int pos_c = ((*iim_list)->cols - *(Haralick_smaller_wndw + 2) - 1)*rand() / RAND_MAX;
						(*im_curr) = (*(*iim_list))(Rect(pos_c, pos_r, *(Haralick_smaller_wndw + 2), *(Haralick_smaller_wndw + 1)));
					}
				}
			}
			else { im_curr = (*iim_list); }

			if (par2_yn == 0) {
				//Mat* co_occurrence_mat_curr = get_co_occurrence_mat(offset, ang_text, num_bins, max_val, min_val, (*iim_list));
				Mat* co_occurrence_mat_curr = get_co_occurrence_mat(offset, ang_text, num_bins, max_val, min_val, im_curr);
				/*for (int l=0; l<num_bins; l++) {
				for (int m=0; m<num_bins; m++) {
				cout << co_occurrence_mat_curr->at<int>(l,m) << "\t";
				}
				cout << endl;
				}
				cout << endl;*/
				get_co_occurrence_mat_props(co_occurrence_mat_curr, *(contrast_vect + count_Haralick), *(energy_vect + count_Haralick),
					*(homogeneity_vect + count_Haralick), *(correlation_vect + count_Haralick));
				co_occurrence_mat_list.push_back(co_occurrence_mat_curr);
			}


			if (par2_yn == 1) {
				// in this case the best matching angle is computed
				int num_ang = 0; 
				for (double a = minang_text; a <= maxang_text; a += angoffset_text)
				{
					num_ang++; 
				}
				double** contrast_mat = new double*[num_offset]; 
				for (int i = 0; i<num_offset; i++)
				{ 
					*(contrast_mat + i) = new double[num_ang]; 
				}
				double** energy_mat = new double*[num_offset];
				for (int i = 0; i<num_offset; i++)
				{
					*(energy_mat + i) = new double[num_ang];
				}
				double** homogeneity_mat = new double*[num_offset];
				for (int i = 0; i<num_offset; i++)
				{
					*(homogeneity_mat + i) = new double[num_ang]; 
				}
				double** correlation_mat = new double*[num_offset];
				for (int i = 0; i<num_offset; i++)
				{
					*(correlation_mat + i) = new double[num_ang]; 
				}
				int* answ_Haralick = new int[num_offset]; 
				int* answ_Haralick_occurrence = new int[num_ang];
				double* mean_Haralick = new double[num_offset]; 
				double* std_Haralick = new double[num_offset];
				double* extr_val_Haralick = new double[num_offset];
				for (int i = 0; i<num_ang; i++)
				{
					*(answ_Haralick_occurrence + i) = 0; 
				}

				for (int i = 0; i<num_offset; i++) {
					cout << "image\t" << count_Haralick << " offset\t" << i << endl;
					int count_ang = 0;
					for (double a = minang_text; a <= maxang_text; a += angoffset_text) 
					{
						Mat* co_occurrence_mat_curr =
							get_co_occurrence_mat(*(offset_list + i), a, num_bins2, max_val2, min_val2, im_curr);
						get_co_occurrence_mat_props(co_occurrence_mat_curr, *(*(contrast_mat + i) + count_ang), *(*(energy_mat + i) + count_ang),
							*(*(homogeneity_mat + i) + count_ang), *(*(correlation_mat + i) + count_ang));
						delete co_occurrence_mat_curr;
						count_ang++;
						//cout << "ang\t" << count_ang << endl;
					}
				}

				for (int i = 0; i<num_offset; i++) {
					get_max_from_vector(*(correlation_mat + i), num_ang, *(answ_Haralick + i), *(extr_val_Haralick + i));
					double* tmp_vect = NULL; int tmp_sz = num_ang;
					erase_el_from_vector(*(correlation_mat + i), tmp_vect, tmp_sz, *(answ_Haralick + i));
					get_mean_std_from_vector(tmp_vect, tmp_sz, *(mean_Haralick + i), *(std_Haralick + i));
					delete tmp_vect;
				}
				for (int i = 0; i<num_offset; i++) {
					(*(answ_Haralick_occurrence + *(answ_Haralick + i)))++;
				}
				int num_occ, ang_extr_curr; bool multiple_occ;
				get_max_from_vector(answ_Haralick_occurrence, num_ang, ang_extr_curr, num_occ, multiple_occ);

				if (!multiple_occ) {
					*(Haralick_answer_fin + count_Haralick) = ang_extr_curr;
					*(Haralick_margin_fin + count_Haralick) = 0;
					int count_ang_extr_curr = 0;
					for (int i = 0; i<num_offset; i++) {
						if (*(answ_Haralick + i) == ang_extr_curr) {
							count_ang_extr_curr++;
							*(Haralick_margin_fin + count_Haralick) += (*(extr_val_Haralick + i) - *(mean_Haralick + i)) / *(std_Haralick + i);
						}
					}
					*(Haralick_margin_fin + count_Haralick) /= count_ang_extr_curr;
				}
				else {
					if (num_occ>3) {
						*(Haralick_answer_fin + count_Haralick) = ang_extr_curr;
						*(Haralick_margin_fin + count_Haralick) = 0;
						int count_ang_extr_curr = 0;
						for (int i = 0; i<num_offset; i++) {
							if (*(answ_Haralick + i) == ang_extr_curr) {
								count_ang_extr_curr++;
								*(Haralick_margin_fin + count_Haralick) += (*(extr_val_Haralick + i) - *(mean_Haralick + i)) / *(std_Haralick + i);
							}
						}
						*(Haralick_margin_fin + count_Haralick) /= count_ang_extr_curr;
					}
					else {
						*(Haralick_answer_fin + count_Haralick) = -1; *(Haralick_margin_fin + count_Haralick) = -1;
					}
				}
				for (int i = 0; i<num_offset; i++) { delete *(contrast_mat + i); }; delete contrast_mat;
				for (int i = 0; i<num_offset; i++) { delete *(energy_mat + i); }; delete energy_mat;
				for (int i = 0; i<num_offset; i++) { delete *(homogeneity_mat + i); }; delete homogeneity_mat;
				for (int i = 0; i<num_offset; i++) { delete *(correlation_mat + i); }; delete correlation_mat;
			}


			if (par2_yn == 2) {
				// in this case offset-related data is compyted, so num_ang should has to be equal to 1
				int num_ang = 0; for (double a = minang_text; a <= maxang_text; a += angoffset_text) { num_ang++; }
				if (num_ang != 1) {
					cout << "par2_yn==2 but num_ang different from 1!!!" << endl;
					exit(1);
				}
				double* contrast_mat = new double[num_offset]; double* energy_mat = new double[num_offset];
				double* homogeneity_mat = new double[num_offset]; double* correlation_mat = new double[num_offset];
				double* answ_Haralick_deriv1 = new double[num_offset - 1]; double* answ_Haralick_deriv2 = new double[num_offset - 2];

				for (int i = 0; i<num_offset; i++) {
					cout << "image\t" << count_Haralick << " offset\t" << i << endl;
					int count_ang = 0;

					// in this case num_ang should has to be equal to 1, so for loop executed only one time
					Mat* co_occurrence_mat_curr =
						get_co_occurrence_mat(*(offset_list + i), minang_text, num_bins2, max_val2, min_val2, im_curr);
					get_co_occurrence_mat_props(co_occurrence_mat_curr, *(contrast_mat + i), *(energy_mat + i),
						*(homogeneity_mat + i), *(correlation_mat + i));
					delete co_occurrence_mat_curr;
				}

				double* answ_Haralick = NULL;
				answ_Haralick = correlation_mat;
				//for (int l=0; l<num_offset; l++) { *(answ_Haralick+l) = *(correlation_mat+l); }

				*(answ_Haralick_deriv1) = (*(answ_Haralick + 1) - *(answ_Haralick)) / (*(offset_list + 1) - *(offset_list));
				for (int i = 1; i<num_offset - 1; i++) {
					*(answ_Haralick_deriv1 + i) = (*(answ_Haralick + i + 1) - *(answ_Haralick + i)) / (*(offset_list + i + 1) - *(offset_list + i));
					*(answ_Haralick_deriv2 + i - 1) = abs((*(answ_Haralick_deriv1 + i) - *(answ_Haralick_deriv1 + i - 1)) /
						(*(offset_list + i) - *(offset_list + i - 1)));
				}
				int pos_max; double answ_Haralick_deriv2_max;

				/*ofstream outfile_tmp;
				outfile_tmp.open("C:/Users/toniocar/Desktop/tempo.txt");
				for (int i=0; i<num_offset-2; i++) {
				outfile_tmp << *(answ_Haralick_deriv2+i) << "\t";
				}
				outfile_tmp << endl;
				outfile_tmp.close();*/

				//int num_max_considered = 1; //THIS IS USER-DEFINED
				//list <int> temp_list; list<int>::iterator itemp_list;
				//get_max_from_vector(answ_Haralick_deriv2, num_offset-2, pos_max, answ_Haralick_deriv2_max); 
				//int curr_size = num_offset-2; double* new_vect = NULL; 
				//int curr_size_offset_list = num_offset; int* new_offset_list = NULL;
				//erase_el_from_vector(answ_Haralick_deriv2, new_vect, curr_size, pos_max);				
				//temp_list.push_back(*(offset_list+pos_max+1));
				//erase_el_from_vector(offset_list, new_offset_list, curr_size_offset_list, pos_max+1);
				//for (int k=0; k<num_max_considered-1; k++) {					
				//	double* old_vect = new_vect; new_vect = NULL;
				//	int* old_offset_list = new_offset_list; new_offset_list = NULL;
				//	get_max_from_vector(old_vect, curr_size, pos_max, answ_Haralick_deriv2_max); 
				//	erase_el_from_vector(old_vect, new_vect, curr_size, pos_max);
				//	delete old_vect; 
				//	temp_list.push_back(*(old_offset_list+pos_max+1));
				//	erase_el_from_vector(old_offset_list, new_offset_list, curr_size_offset_list, pos_max+1);
				//	delete old_offset_list;
				//}
				//delete new_vect; delete new_offset_list;


				get_max_from_vector(answ_Haralick_deriv2, num_offset - 2, pos_max, answ_Haralick_deriv2_max);

				list <int> temp_list;
				list<int>::iterator itemp_list = temp_list.begin();
				if (answ_Haralick_deriv2_max>.001) {
					for (int i = 0; i<num_offset - 2; i++) { //CHANGE /10*2 TO OTHER VALUES IF NEEDED!		
						if (*(answ_Haralick_deriv2 + i)>answ_Haralick_deriv2_max / 10 * 2) { temp_list.push_back(*(offset_list + i + 1)); }
					}
				}

				*(size_Haralick_answers_fin + count_Haralick) = temp_list.size();
				*(Haralick_answers_fin + count_Haralick) = new int[temp_list.size()];
				itemp_list = temp_list.begin();
				int tmp_i = 0;
				while (itemp_list != temp_list.end()) {
					*(*(Haralick_answers_fin + count_Haralick) + tmp_i) = (*itemp_list);
					itemp_list++; tmp_i++;
				}

				temp_list.clear();
				delete contrast_mat; delete energy_mat; delete homogeneity_mat; delete correlation_mat;
				delete answ_Haralick_deriv1; delete answ_Haralick_deriv2;
				/*cout << *(size_Haralick_answers_fin+count_Haralick) << endl;
				for (int i=0; i<*(size_Haralick_answers_fin+count_Haralick); i++) {
				cout << "\t" << *(*(Haralick_answers_fin+count_Haralick)+i);
				}
				cout << endl;*/
			}





			if (par2_yn == 3) {

				int im_row_sz = im_curr->rows; int im_col_sz = im_curr->cols;
				int tile_row_num = im_row_sz / tile_row_sz; int tile_col_num = im_col_sz / tile_col_sz;
				int tile_row_rem = im_row_sz - tile_row_sz*tile_row_num; int tile_col_rem = im_col_sz - tile_col_sz*tile_col_num;

				*(num_row_tiles_Haralick_fin + count_Haralick) = tile_row_num; *(num_col_tiles_Haralick_fin + count_Haralick) = tile_col_num;

				cout << "Num tiles: " << tile_row_num*tile_col_num << endl;

				// angle data structure
				*(Haralick_answer_ang_fin + count_Haralick) = new int[tile_row_num*tile_col_num];
				*(Haralick_margin_ang_fin + count_Haralick) = new double[tile_row_num*tile_col_num];

				// offset data structure
				*(Haralick_answers_offset_fin + count_Haralick) = new int*[tile_row_num*tile_col_num];

				int curr_tile_num = 0;
				int tile_row_rem_curr = 0, tile_col_rem_curr = 0;

				// if save_Haralick_feat_yn==1, write tiled image code
				int save_Haralick_feat_yn = atoi(save_Haralick_feat.c_str());
				if (save_Haralick_feat_yn == 1) {

					Mat* im_curr_clone = new Mat; *im_curr_clone = im_curr->clone();
					for (int i_tile = 0; i_tile<tile_row_num; i_tile++) {
						if (i_tile == tile_row_num - 1) { tile_row_rem_curr = tile_row_rem; }
						for (int j_tile = 0; j_tile<tile_col_num; j_tile++) {
							if (j_tile == tile_col_num - 1) { tile_col_rem_curr = tile_col_rem; }
							Mat *curr_tile_clone = new Mat;
							*curr_tile_clone =
								(*im_curr_clone)(Rect(j_tile*tile_col_sz, i_tile*tile_row_sz, tile_col_sz + tile_col_rem_curr, tile_row_sz + tile_row_rem_curr));
							int sz_row_curr_tile_clone = curr_tile_clone->rows;
							int sz_col_curr_tile_clone = curr_tile_clone->cols;
							Mat *mask = new Mat(sz_row_curr_tile_clone, sz_col_curr_tile_clone, CV_16U, 0.);
							(*mask)(Rect(1, 1, sz_col_curr_tile_clone - 2, sz_row_curr_tile_clone - 2)) = 1;
							multiply(*curr_tile_clone, *mask, *curr_tile_clone);
							delete curr_tile_clone, mask;
							tile_col_rem_curr = 0;
							cout << "j_tile " << j_tile << endl;
						}
						cout << "i_tile " << i_tile << endl;
						tile_row_rem_curr = 0;
					}
					const char* Haralick_path_save_ch = new char[50];
					string Haralick_path_save = Haralick_path + res_Haralick_path;
					Haralick_path_save_ch = Haralick_path_save.c_str();
					mkdir(Haralick_path_save_ch);
					int filenamelength_tmp = (*ifilename_list).size();
					string tile_row_sz_convert_str; ostringstream tile_row_sz_convert;
					tile_row_sz_convert << tile_row_sz;  tile_row_sz_convert_str = tile_row_sz_convert.str();
					string tile_col_sz_convert_str; ostringstream tile_col_sz_convert;
					tile_col_sz_convert << tile_col_sz;  tile_col_sz_convert_str = tile_col_sz_convert.str();
					String filename_clone = Haralick_path_save + "tiled_" + (*ifilename_list).substr(0, filenamelength_tmp - 4) + "_"
						+ tile_row_sz_convert_str + "_" + tile_col_sz_convert_str + ".tif";
					imwrite(filename_clone, *im_curr_clone);
					delete im_curr_clone;

				}

				for (int i_tile = 0; i_tile<tile_row_num; i_tile++) { // (int i_tile=12; i_tile<13; i_tile++) { // 
					if (i_tile == tile_row_num - 1) { tile_row_rem_curr = tile_row_rem; }
					for (int j_tile = 0; j_tile<tile_col_num; j_tile++) { // (int j_tile=14; j_tile<16; j_tile++) { // 
						//Mat curr_tile = (*im_curr)(Rect(i*tile_col_sz,j*tile_row_sz,tile_col_sz,tile_row_sz));						
						if (j_tile == tile_col_num - 1) { tile_col_rem_curr = tile_col_rem; }

						Mat *curr_tile = new Mat;
						*curr_tile =
							(*im_curr)(Rect(j_tile*tile_col_sz, i_tile*tile_row_sz, tile_col_sz + tile_col_rem_curr, tile_row_sz + tile_row_rem_curr));
						Mat *curr_tile2 = new Mat; // this matrix was defined because of a bug in opencv function "copyMakeBorder" used in 
						// my "get_co_occurrence_mat" function: when padding the tile, some values from adjacent
						// tiles are included instead of padding with zero values, as requested
						curr_tile->copyTo(*curr_tile2);

						// angle computation
						int num_ang = 0; for (double a = minang_text; a <= maxang_text; a += angoffset_text) { num_ang++; }
						double** contrast_mat = new double*[num_offset]; for (int i = 0; i<num_offset; i++) { *(contrast_mat + i) = new double[num_ang]; }
						double** energy_mat = new double*[num_offset]; for (int i = 0; i<num_offset; i++) { *(energy_mat + i) = new double[num_ang]; }
						double** homogeneity_mat = new double*[num_offset]; for (int i = 0; i<num_offset; i++) { *(homogeneity_mat + i) = new double[num_ang]; }
						double** correlation_mat = new double*[num_offset]; for (int i = 0; i<num_offset; i++) { *(correlation_mat + i) = new double[num_ang]; }
						int* answ_Haralick = new int[num_offset]; int* answ_Haralick_occurrence = new int[num_ang];
						double* mean_Haralick = new double[num_offset]; double* std_Haralick = new double[num_offset];
						double* extr_val_Haralick = new double[num_offset];
						for (int i = 0; i<num_ang; i++) { *(answ_Haralick_occurrence + i) = 0; }

						for (int i = 0; i<num_offset; i++) {
							//cout << "image\t" << count_Haralick << " offset\t" << i <<  " tile\t" << curr_tile_num << endl;
							int count_ang = 0;
							for (double a = minang_text; a <= maxang_text; a += angoffset_text) {
								Mat* co_occurrence_mat_curr =
									get_co_occurrence_mat(*(offset_list + i), a, num_bins2, max_val2, min_val2, curr_tile2);

								// THIS CODE BELOW PRINTS SELECTED COO OCCURRENCE MATRIX FOR QUICK CHECK
								if (0 & (i_tile == 12)&(j_tile == 15)&(i == 5)&(count_ang == 5)) {
									cout << "type " << co_occurrence_mat_curr->type() << endl;
									cout << "sum " << sum(*co_occurrence_mat_curr)[0] << endl;
									for (int l = 0; l<num_bins2; l++) {
										for (int m = 0; m<num_bins2; m++) {
											cout << co_occurrence_mat_curr->at<int>(l, m) << "\t";
										}
										cout << endl;
									}
								}
								get_co_occurrence_mat_props(co_occurrence_mat_curr, *(*(contrast_mat + i) + count_ang), *(*(energy_mat + i) + count_ang),
									*(*(homogeneity_mat + i) + count_ang), *(*(correlation_mat + i) + count_ang));
								delete co_occurrence_mat_curr;
								count_ang++;
								//cout << "ang\t" << count_ang << endl;
							}
						}

						for (int i = 0; i<num_offset; i++) {
							get_max_from_vector(*(correlation_mat + i), num_ang, *(answ_Haralick + i), *(extr_val_Haralick + i));
							double* tmp_vect = NULL; int tmp_sz = num_ang;
							erase_el_from_vector(*(correlation_mat + i), tmp_vect, tmp_sz, *(answ_Haralick + i));
							get_mean_std_from_vector(tmp_vect, tmp_sz, *(mean_Haralick + i), *(std_Haralick + i));
							delete tmp_vect;
						}
						for (int i = 0; i<num_offset; i++) {
							(*(answ_Haralick_occurrence + *(answ_Haralick + i)))++;
						}
						int num_occ, ang_extr_curr; bool multiple_occ;
						get_max_from_vector(answ_Haralick_occurrence, num_ang, ang_extr_curr, num_occ, multiple_occ);

						if (!multiple_occ) {

							*(*(Haralick_answer_ang_fin + count_Haralick) + curr_tile_num) = ang_extr_curr;
							*(*(Haralick_margin_ang_fin + count_Haralick) + curr_tile_num) = 0;
							int count_ang_extr_curr = 0;
							for (int i = 0; i<num_offset; i++) {
								if (*(answ_Haralick + i) == ang_extr_curr) {
									count_ang_extr_curr++;
									*(*(Haralick_margin_ang_fin + count_Haralick) + curr_tile_num) +=
										(*(extr_val_Haralick + i) - *(mean_Haralick + i)) / *(std_Haralick + i);
								}
							}
							*(*(Haralick_margin_ang_fin + count_Haralick) + curr_tile_num) /= count_ang_extr_curr;
						}
						else {
							if (num_occ>3) {
								*(*(Haralick_answer_ang_fin + count_Haralick) + curr_tile_num) = ang_extr_curr;
								*(*(Haralick_margin_ang_fin + count_Haralick) + curr_tile_num) = 0;
								int count_ang_extr_curr = 0;
								for (int i = 0; i<num_offset; i++) {
									if (*(answ_Haralick + i) == ang_extr_curr) {
										count_ang_extr_curr++;
										*(*(Haralick_margin_ang_fin + count_Haralick) + curr_tile_num) +=
											(*(extr_val_Haralick + i) - *(mean_Haralick + i)) / *(std_Haralick + i);
									}
								}
								*(*(Haralick_margin_ang_fin + count_Haralick) + curr_tile_num) /= count_ang_extr_curr;
							}
							else {
								*(*(Haralick_answer_ang_fin + count_Haralick) + curr_tile_num) = -1;
								*(*(Haralick_margin_ang_fin + count_Haralick) + curr_tile_num) = -1;
							}
						}
						for (int i = 0; i<num_offset; i++) { delete *(contrast_mat + i); }; delete contrast_mat;
						for (int i = 0; i<num_offset; i++) { delete *(energy_mat + i); }; delete energy_mat;
						for (int i = 0; i<num_offset; i++) { delete *(homogeneity_mat + i); }; delete homogeneity_mat;
						for (int i = 0; i<num_offset; i++) { delete *(correlation_mat + i); }; delete correlation_mat;

						// offset computation
						*(*(Haralick_answers_offset_fin + count_Haralick) + curr_tile_num) = new int[4];
						if (*(*(Haralick_answer_ang_fin + count_Haralick) + curr_tile_num) == -1) {
							// no angle previously identified
							*(*(*(Haralick_answers_offset_fin + count_Haralick) + curr_tile_num) + 0) = -1;
							*(*(*(Haralick_answers_offset_fin + count_Haralick) + curr_tile_num) + 1) = -1;
							*(*(*(Haralick_answers_offset_fin + count_Haralick) + curr_tile_num) + 2) = -1;
							*(*(*(Haralick_answers_offset_fin + count_Haralick) + curr_tile_num) + 3) = -1;

						}
						else {
							// angle previously identified
							double** contrast_mat = new double*[2]; double** energy_mat = new double*[2];
							double** homogeneity_mat = new double*[2]; double** correlation_mat = new double*[2];
							double** answ_Haralick_deriv1 = new double*[2]; double** answ_Haralick_deriv2 = new double*[2];

							//angle previously stored in *(*(Haralick_answer_ang_fin+count_Haralick)+curr_tile_num)
							double *ang_curr = new double[2];
							*(ang_curr) = *(*(Haralick_answer_ang_fin + count_Haralick) + curr_tile_num)*angoffset_text + minang_text;
							// perpendicular to previous angle
							if (*(ang_curr)<0) { *(ang_curr + 1) = *(ang_curr)+90; }
							else { *(ang_curr + 1) = *(ang_curr)-90; }

							for (int j = 0; j<2; j++) {
								*(contrast_mat + j) = new double[num_offset]; *(energy_mat + j) = new double[num_offset];
								*(homogeneity_mat + j) = new double[num_offset]; *(correlation_mat + j) = new double[num_offset];
								*(answ_Haralick_deriv1 + j) = new double[num_offset - 1]; *(answ_Haralick_deriv2 + j) = new double[num_offset - 2];
								for (int i = 0; i<num_offset; i++) {
									//cout << "image\t" << count_Haralick << " offset\t" << i <<  " tile\t" << curr_tile_num << endl;

									// in this case num_ang should has to be equal to 1, so for loop executed only one time
									Mat* co_occurrence_mat_curr =
										get_co_occurrence_mat(*(offset_list + i), *(ang_curr + j),
										num_bins2, max_val2, min_val2, curr_tile2);
									get_co_occurrence_mat_props(co_occurrence_mat_curr, *(*(contrast_mat + j) + i), *(*(energy_mat + j) + i),
										*(*(homogeneity_mat + j) + i), *(*(correlation_mat + j) + i));
									delete co_occurrence_mat_curr;

								}
							}

							double** answ_Haralick = NULL;
							answ_Haralick = correlation_mat;
							for (int j = 0; j<2; j++) {
								// 2 computations: one for angl, one for its perpendicular
								*(*(answ_Haralick_deriv1 + j)) =
									(*(*(answ_Haralick + j) + 1) - *(*(answ_Haralick + j))) / (*(offset_list + 1) - *(offset_list));
								for (int i = 1; i<num_offset - 1; i++) {
									*(*(answ_Haralick_deriv1 + j) + i) = (*(*(answ_Haralick + j) + i + 1) - *(*(answ_Haralick + j) + i)) /
										(*(offset_list + i + 1) - *(offset_list + i));
									*(*(answ_Haralick_deriv2 + j) + i - 1) = abs((*(*(answ_Haralick_deriv1 + j) + i) - *(*(answ_Haralick_deriv1 + j) + i - 1)) /
										(*(offset_list + i) - *(offset_list + i - 1)));
								}
								int pos_max; double answ_Haralick_deriv2_max;
								get_max_from_vector(*(answ_Haralick_deriv2 + j), num_offset - 2, pos_max, answ_Haralick_deriv2_max);
								// low threshold computation
								list <int> temp_list_low;
								if (answ_Haralick_deriv2_max>.001) {
									for (int i = 0; i<num_offset - 2; i++) { //CHANGE /10*2 TO OTHER VALUES IF NEEDED!												
										if (*(*(answ_Haralick_deriv2 + j) + i)>answ_Haralick_deriv2_max / 10 * 2) { temp_list_low.push_back(*(offset_list + i + 1)); }
									}
								}
								// high threshold computation
								list <int> temp_list_high;
								if (answ_Haralick_deriv2_max>.001) {
									for (int i = 0; i<num_offset - 2; i++) { //CHANGE /10*8 TO OTHER VALUES IF NEEDED!		
										if (*(*(answ_Haralick_deriv2 + j) + i)>answ_Haralick_deriv2_max / 10 * 8) { temp_list_high.push_back(*(offset_list + i + 1)); }
									}
								}

								// THIS CODE BELOW PRINTS OFFSET VALUES AND ANGLE VALUES THAT ARE SELECTED FOR EACH TILE
								if (0) {
									cout << temp_list_low.size() << endl;
									list<int>::iterator itemp_list = temp_list_low.begin();
									while (itemp_list != temp_list_low.end()) {
										cout << (*itemp_list) << " ";
										itemp_list++;
									}
									cout << endl;
									cout << temp_list_high.size() << endl;
									itemp_list = temp_list_high.begin();
									while (itemp_list != temp_list_high.end()) {
										cout << (*itemp_list) << " ";
										itemp_list++;
									}
									cout << endl;
									if (j == 1) {
										if (*(*(Haralick_margin_ang_fin + count_Haralick) + curr_tile_num) == -1) {
											cout << *(*(Haralick_margin_ang_fin + count_Haralick) + curr_tile_num) << " "
												<< *(*(Haralick_answer_ang_fin + count_Haralick) + curr_tile_num) << endl;
										}
										else
										{
											cout << *(*(Haralick_margin_ang_fin + count_Haralick) + curr_tile_num) << " "
												<< *(*(Haralick_answer_ang_fin + count_Haralick) + curr_tile_num)*angoffset_text + minang_text << endl;
										}
									}
								}

								if ((temp_list_low.size()>0)&(temp_list_low.size()<num_offset - 2)&
									(temp_list_high.size()>0)&(temp_list_high.size()<num_offset - 2)) {
									list<int>::iterator itemp_list = temp_list_low.begin();
									*(*(*(Haralick_answers_offset_fin + count_Haralick) + curr_tile_num) + 2 * j + 0) = (*itemp_list);
								}
								else { *(*(*(Haralick_answers_offset_fin + count_Haralick) + curr_tile_num) + 2 * j + 0) = 0; }

								if ((temp_list_low.size()>0)&(temp_list_low.size()<num_offset - 2)&
									(temp_list_high.size()>0)&(temp_list_high.size()<num_offset - 2)) {
									list<int>::iterator itemp_list = temp_list_high.begin();
									*(*(*(Haralick_answers_offset_fin + count_Haralick) + curr_tile_num) + 2 * j + 1) = (*itemp_list);
								}
								else { *(*(*(Haralick_answers_offset_fin + count_Haralick) + curr_tile_num) + 2 * j + 1) = 0; }
								temp_list_low.clear();
								temp_list_high.clear();
							}
							delete contrast_mat; delete energy_mat; delete homogeneity_mat; delete correlation_mat;
							delete answ_Haralick_deriv1; delete answ_Haralick_deriv2;

						}


						delete curr_tile, curr_tile2;
						tile_col_rem_curr = 0;
						cout << endl << "tile " << curr_tile_num << " of " << tile_row_num*tile_col_num << " computed" << endl << endl;
						curr_tile_num++;
					}
					tile_row_rem_curr = 0;
				}

			}

			iim_list++; count_Haralick++; ifilename_list++;
		}

		int save_Haralick_feat_yn = atoi(save_Haralick_feat.c_str());
		if ((save_Haralick_feat_yn == 1) && (par2_yn == 0))
		{
			const char* Haralick_path_save_ch = new char[50];
			string Haralick_path_save = Haralick_path + res_Haralick_path;
			Haralick_path_save_ch = Haralick_path_save.c_str();
			mkdir(Haralick_path_save_ch);
			string tmp_str, name_tmp = ""; ostringstream tmp_convert;
			tmp_convert << offset;  tmp_str = tmp_convert.str(); name_tmp = name_tmp + tmp_str + "_"; tmp_convert.str(""); tmp_convert.clear();
			tmp_convert << ang_text;  tmp_str = tmp_convert.str(); name_tmp = name_tmp + tmp_str + "_"; tmp_convert.str(""); tmp_convert.clear();
			tmp_convert << num_bins;  tmp_str = tmp_convert.str(); name_tmp = name_tmp + tmp_str + "_"; tmp_convert.str(""); tmp_convert.clear();
			tmp_convert << max_val;  tmp_str = tmp_convert.str(); name_tmp = name_tmp + tmp_str + "_"; tmp_convert.str(""); tmp_convert.clear();
			tmp_convert << min_val;  tmp_str = tmp_convert.str(); name_tmp = name_tmp + tmp_str; tmp_convert.str(""); tmp_convert.clear();
			string namefile_string_out = Haralick_path_save + "Haralick_data_" + name_tmp + ".txt";
			const char* namefile_out = new char[50];
			namefile_out = namefile_string_out.c_str();
			ofstream outfile;
			outfile.open(namefile_out);

			ifilename_list = filename_list.begin();
			list<Mat*>::iterator ico_occurrence_mat_list = co_occurrence_mat_list.begin();
			count_Haralick = 0;
			while (ifilename_list != filename_list.end()) {
				outfile << *(contrast_vect + count_Haralick) << "\t" << *(energy_vect + count_Haralick) << "\t" <<
					*(homogeneity_vect + count_Haralick) << "\t" << *(correlation_vect + count_Haralick) << "\t" <<
					(*ifilename_list) << endl;

				int pos1 = (*ifilename_list).find("ang"); int pos2 = (*ifilename_list).find(".tiff");
				String name_tmp1 = (*ifilename_list).substr(pos1, pos2 - pos1);
				string namefile_coocc_string_out = Haralick_path_save + "coocc_mat_" + name_tmp + "_" + name_tmp1 + ".tif";
				//string namefile_coocc_string_out = Haralick_path_save+"coocc_mat_" + name_tmp + ".tif";
				imwrite(namefile_coocc_string_out, *(*ico_occurrence_mat_list));

				ifilename_list++; count_Haralick++; ico_occurrence_mat_list++;
			}
			outfile.close();
		}

		if ((save_Haralick_feat_yn == 1) && (par2_yn == 1)) {
			const char* Haralick_path_save_ch = new char[50];
			string Haralick_path_save = Haralick_path + res_Haralick_path;
			Haralick_path_save_ch = Haralick_path_save.c_str();
			mkdir(Haralick_path_save_ch);
			string tmp_str, name_tmp = ""; ostringstream tmp_convert;
			for (int k = 0; k<num_offset; k++) {
				tmp_convert << *(offset_list + k);  tmp_str = tmp_convert.str(); name_tmp = name_tmp + tmp_str + "_"; tmp_convert.str(""); tmp_convert.clear();
			}
			tmp_convert << angoffset_text;  tmp_str = tmp_convert.str(); name_tmp = name_tmp + tmp_str + "_"; tmp_convert.str(""); tmp_convert.clear();
			tmp_convert << minang_text;  tmp_str = tmp_convert.str(); name_tmp = name_tmp + tmp_str + "_"; tmp_convert.str(""); tmp_convert.clear();
			tmp_convert << maxang_text;  tmp_str = tmp_convert.str(); name_tmp = name_tmp + tmp_str + "_"; tmp_convert.str(""); tmp_convert.clear();
			tmp_convert << num_bins2;  tmp_str = tmp_convert.str(); name_tmp = name_tmp + tmp_str + "_"; tmp_convert.str(""); tmp_convert.clear();
			tmp_convert << max_val2;  tmp_str = tmp_convert.str(); name_tmp = name_tmp + tmp_str + "_"; tmp_convert.str(""); tmp_convert.clear();
			tmp_convert << min_val2;  tmp_str = tmp_convert.str(); name_tmp = name_tmp + tmp_str; tmp_convert.str(""); tmp_convert.clear();
			/*ifilename_list = filename_list.begin();
			int pos1 = (*ifilename_list).find("lgap"); int pos2 = (*ifilename_list).find("ang");
			string name_cropped = (*ifilename_list).substr(pos1+5,pos2-(pos1+5)-1);
			string namefile_string_out = Haralick_path_save+"Haralick_data_im-sp_" + name_cropped + "_Ha-sp_" + name_tmp + ".txt";*/
			string namefile_string_out = Haralick_path_save + "Haralick_correl_" + name_tmp + ".txt";
			const char* namefile_out = new char[50];
			namefile_out = namefile_string_out.c_str();
			ofstream outfile;
			outfile.open(namefile_out);

			ifilename_list = filename_list.begin();
			//list<Mat*>::iterator ico_occurrence_mat_list = co_occurrence_mat_list.begin();
			count_Haralick = 0;
			while (ifilename_list != filename_list.end()) {
				outfile << *(Haralick_answer_fin + count_Haralick) << "\t" << *(Haralick_margin_fin + count_Haralick) << "\t" <<
					(*ifilename_list) << endl;

				//int pos1 = (*ifilename_list).find("ang"); int pos2 = (*ifilename_list).find(".tiff");
				//String name_tmp1 = (*ifilename_list).substr(pos1,pos2-pos1);
				//string namefile_coocc_string_out = Haralick_path_save+"coocc_mat_" + name_tmp + "_" + name_tmp1 + ".tif";
				//imwrite(namefile_coocc_string_out,* (*ico_occurrence_mat_list));

				ifilename_list++; count_Haralick++; //ico_occurrence_mat_list++;
			}
			outfile.close();
		}

		if ((save_Haralick_feat_yn == 1) && (par2_yn == 2)) {
			const char* Haralick_path_save_ch = new char[50];
			string Haralick_path_save = Haralick_path + res_Haralick_path;
			Haralick_path_save_ch = Haralick_path_save.c_str();
			mkdir(Haralick_path_save_ch);
			string tmp_str, name_tmp = ""; ostringstream tmp_convert;
			int k = 0;
			tmp_convert << *(offset_list + k);  tmp_str = tmp_convert.str(); name_tmp = name_tmp + tmp_str + "_"; tmp_convert.str(""); tmp_convert.clear();
			k = num_offset - 1;
			tmp_convert << *(offset_list + k);  tmp_str = tmp_convert.str(); name_tmp = name_tmp + tmp_str + "_"; tmp_convert.str(""); tmp_convert.clear();
			/*for (int k=0; k<num_offset; k++) {
			tmp_convert << *(offset_list+k);  tmp_str = tmp_convert.str(); name_tmp = name_tmp + tmp_str + "_"; tmp_convert.str(""); tmp_convert.clear();
			}*/
			tmp_convert << angoffset_text;  tmp_str = tmp_convert.str(); name_tmp = name_tmp + tmp_str + "_"; tmp_convert.str(""); tmp_convert.clear();
			tmp_convert << minang_text;  tmp_str = tmp_convert.str(); name_tmp = name_tmp + tmp_str + "_"; tmp_convert.str(""); tmp_convert.clear();
			tmp_convert << maxang_text;  tmp_str = tmp_convert.str(); name_tmp = name_tmp + tmp_str + "_"; tmp_convert.str(""); tmp_convert.clear();
			tmp_convert << num_bins2;  tmp_str = tmp_convert.str(); name_tmp = name_tmp + tmp_str + "_"; tmp_convert.str(""); tmp_convert.clear();
			tmp_convert << max_val2;  tmp_str = tmp_convert.str(); name_tmp = name_tmp + tmp_str + "_"; tmp_convert.str(""); tmp_convert.clear();
			tmp_convert << min_val2;  tmp_str = tmp_convert.str(); name_tmp = name_tmp + tmp_str; tmp_convert.str(""); tmp_convert.clear();
			/*ifilename_list = filename_list.begin();
			int pos1 = (*ifilename_list).find("lgap"); int pos2 = (*ifilename_list).find("ang");
			string name_cropped = (*ifilename_list).substr(pos1+5,pos2-(pos1+5)-1);
			string namefile_string_out = Haralick_path_save+"Haralick_data_im-sp_" + name_cropped + "_Ha-sp_" + name_tmp + ".txt";*/
			string namefile_string_out = Haralick_path_save + "Haralick_correl_" + name_tmp + ".txt";
			const char* namefile_out = new char[50];
			namefile_out = namefile_string_out.c_str();
			ofstream outfile;
			outfile.open(namefile_out);

			ifilename_list = filename_list.begin();
			//list<Mat*>::iterator ico_occurrence_mat_list = co_occurrence_mat_list.begin();
			count_Haralick = 0;
			while (ifilename_list != filename_list.end()) {

				//*(size_Haralick_answers_fin+count_Haralick) = temp_list.size();
				//*(Haralick_answers_fin+count_Haralick) = new int(temp_list.size());
				outfile << (*ifilename_list);
				for (int i = 0; i<*(size_Haralick_answers_fin + count_Haralick); i++) {
					//cout << "\t" << *(*(Haralick_answers_fin+count_Haralick)+i);
					outfile << "\t" << *(*(Haralick_answers_fin + count_Haralick) + i);
				}
				//cout << endl;
				outfile << endl;

				ifilename_list++; count_Haralick++; //ico_occurrence_mat_list++;
			}
			outfile.close();
		}


		if ((save_Haralick_feat_yn == 1) && (par2_yn == 3)) {
			const char* Haralick_path_save_ch = new char[50];
			string Haralick_path_save = Haralick_path + res_Haralick_path;
			Haralick_path_save_ch = Haralick_path_save.c_str();
			mkdir(Haralick_path_save_ch);
			string tmp_str, name_tmp = ""; ostringstream tmp_convert;
			int k = 0;
			tmp_convert << *(offset_list + k);  tmp_str = tmp_convert.str(); name_tmp = name_tmp + tmp_str + "_"; tmp_convert.str(""); tmp_convert.clear();
			k = num_offset - 1;
			tmp_convert << *(offset_list + k);  tmp_str = tmp_convert.str(); name_tmp = name_tmp + tmp_str + "_"; tmp_convert.str(""); tmp_convert.clear();
			/*for (int k=0; k<num_offset; k++) {
			tmp_convert << *(offset_list+k);  tmp_str = tmp_convert.str(); name_tmp = name_tmp + tmp_str + "_"; tmp_convert.str(""); tmp_convert.clear();
			}*/
			tmp_convert << angoffset_text;  tmp_str = tmp_convert.str(); name_tmp = name_tmp + tmp_str + "_"; tmp_convert.str(""); tmp_convert.clear();
			tmp_convert << minang_text;  tmp_str = tmp_convert.str(); name_tmp = name_tmp + tmp_str + "_"; tmp_convert.str(""); tmp_convert.clear();
			tmp_convert << maxang_text;  tmp_str = tmp_convert.str(); name_tmp = name_tmp + tmp_str + "_"; tmp_convert.str(""); tmp_convert.clear();
			tmp_convert << num_bins2;  tmp_str = tmp_convert.str(); name_tmp = name_tmp + tmp_str + "_"; tmp_convert.str(""); tmp_convert.clear();
			tmp_convert << max_val2;  tmp_str = tmp_convert.str(); name_tmp = name_tmp + tmp_str + "_"; tmp_convert.str(""); tmp_convert.clear();
			tmp_convert << min_val2;  tmp_str = tmp_convert.str(); name_tmp = name_tmp + tmp_str; tmp_convert.str(""); tmp_convert.clear();

			string tile_row_sz_convert_str; ostringstream tile_row_sz_convert;
			tile_row_sz_convert << tile_row_sz;  tile_row_sz_convert_str = tile_row_sz_convert.str();
			string tile_col_sz_convert_str; ostringstream tile_col_sz_convert;
			tile_col_sz_convert << tile_col_sz;  tile_col_sz_convert_str = tile_col_sz_convert.str();

			ifilename_list = filename_list.begin();

			count_Haralick = 0;
			while (ifilename_list != filename_list.end()) {

				int tmplength = (*ifilename_list).size();
				string namefile_string_out = Haralick_path_save + (*ifilename_list).substr(0, tmplength - 4) + "_" +
					tile_row_sz_convert_str + "_" + tile_col_sz_convert_str + "_" + name_tmp + ".txt";
				const char* namefile_out = new char[50];
				namefile_out = namefile_string_out.c_str();
				ofstream outfile;
				outfile.open(namefile_out);

				int curr_tile_num = 0;
				for (int i = 0; i<*(num_row_tiles_Haralick_fin + count_Haralick); i++) {
					string curr_i = "";
					tmp_convert << i + 1;  curr_i = tmp_convert.str(); tmp_convert.str(""); tmp_convert.clear();
					for (int j = 0; j<*(num_col_tiles_Haralick_fin + count_Haralick); j++) {
						if (*(*(Haralick_margin_ang_fin + count_Haralick) + curr_tile_num) == -1) {
							outfile << (*(*(Haralick_answer_ang_fin + count_Haralick) + curr_tile_num)) << "\t";
						}
						else {
							outfile << (*(*(Haralick_answer_ang_fin + count_Haralick) + curr_tile_num))*angoffset_text + minang_text << "\t";
						}
						outfile << *(*(Haralick_margin_ang_fin + count_Haralick) + curr_tile_num) << "\t"
							<< *(*(*(Haralick_answers_offset_fin + count_Haralick) + curr_tile_num) + 0) << "\t"
							<< *(*(*(Haralick_answers_offset_fin + count_Haralick) + curr_tile_num) + 1) << "\t"
							<< *(*(*(Haralick_answers_offset_fin + count_Haralick) + curr_tile_num) + 2) << "\t"
							<< *(*(*(Haralick_answers_offset_fin + count_Haralick) + curr_tile_num) + 3) << "\t";
						curr_tile_num++;
						tmp_convert << j + 1;  tmp_str = tmp_convert.str(); tmp_convert.str(""); tmp_convert.clear();
						outfile << "TILE_r" << curr_i << "c" << tmp_str << "\t";
					}
					outfile << endl;
				}
				outfile.close();

				ifilename_list++; count_Haralick++;

			}

		}


	}

	// TEMPORARY CODE TO READ AND MODIFY FILES
	if (compute_Haralick_feat_yn == -1) {

		string filename_path = "C:/Users/toniocar/Desktop/Texture_material/texture_VS_project/config_text_proj_mult.txt";
		int filename_path_length = filename_path.length();
		const char* filename_path_ch = new char[200];
		filename_path_ch = filename_path.c_str();
		ifstream infile;
		infile.open(filename_path_ch);

		int pos_modify = 104; // use MS Word for word count
		int num_values = 17; // how many values to sub at above position?
		string* subs = new string[num_values];
		subs[0] = "0"; // define values
		subs[1] = "11.25"; subs[2] = "-11.25";
		subs[3] = "22.5"; subs[4] = "-22.5";
		subs[5] = "33.75"; subs[6] = "-33.75";
		subs[7] = "45"; subs[8] = "-45";
		subs[9] = "56.25"; subs[10] = "-56.25";
		subs[11] = "67.5"; subs[12] = "-67.5";
		subs[13] = "78.75"; subs[14] = "-78.75";
		subs[15] = "90"; subs[16] = "-90";


		ofstream* outfile_list = new ofstream[num_values];
		string tmp_init = filename_path.substr(0, filename_path_length - 4);
		for (int i = 0; i<num_values; i++) {
			ostringstream i_convert; i_convert << i + 1;  string i_str = i_convert.str();
			string tmp_full = tmp_init + "_" + i_str + ".txt";
			const char* namefile_out = new char[200]; namefile_out = tmp_full.c_str();
			(*(outfile_list + i)).open(namefile_out);
		}

		string tmp_read; string temp_line;
		int pos_word = 0;
		while (getline(infile, temp_line)) {
			stringstream ls(temp_line);
			while (ls >> tmp_read) {
				if (pos_word != pos_modify - 1) {
					for (int i = 0; i<num_values; i++) {
						(*(outfile_list + i)) << tmp_read; (*(outfile_list + i)) << " ";
					}
				}
				else {
					for (int i = 0; i<num_values; i++) {
						(*(outfile_list + i)) << subs[i]; (*(outfile_list + i)) << " ";
					}
				}
				pos_word++;
			}
			for (int i = 0; i<num_values; i++) {
				(*(outfile_list + i)) << endl;
			}
		}
		for (int i = 0; i<num_values; i++) {
			(*(outfile_list + i)).close();
		}
	}

	// TEMPORARY CODE TO CREATE EIGENTEXTURES (FROM MARCIN)
	if (compute_Haralick_feat_yn == -2) {

		string FileName, FolderName, FileNameBase, FileNameExtension;
		FolderName = "C:/Users/toniocar/Desktop/Texture_material/texture_VS_project/Input_images/eigentextures_final/seed_tmp/";
		const char* FolderName_ch = new char[100]; FolderName_ch = FolderName.c_str(); mkdir(FolderName_ch);
		FileNameBase = "Dots"; FileNameExtension = ".tiff";
		// save display options
		bool saveResult = 1; bool saveKernel = 0;
		// Image size 
		int maxX = 100; int maxY = 100;
		// dots params
		int offsetX = 22; int offsetY = 22; int spacingX = 54; int spacingY = 54;
		// krnel rotations params
		float angleStep = 11.25; float angleMax = 90;
		// kernel params
		int kernelLineLegth = 43; int sidebandCount = 5; int tipSchape = 0; int kernelStyle = 0;

		//Base kernel definition
		Mat Kernel;
		int maxKernelX = kernelLineLegth + 2 * sidebandCount;
		int maxKernelY = kernelLineLegth + 2 * sidebandCount;
		int kernelBorderY = sidebandCount;

		Kernel = Mat::zeros(maxKernelY, maxKernelX, CV_32F);

		int middleKernelX = maxKernelX / 2; int middleKernelY = maxKernelY / 2;

		float *wKernel = (float*)Kernel.data;

		for (int y = 0; y < maxKernelY; y++)
		{
			for (int x = 0; x < maxKernelX; x++)
			{
				int sidebandNR = abs(x - middleKernelX);
				if ((y >= kernelBorderY + sidebandNR * tipSchape) & (y < maxKernelY - kernelBorderY - sidebandNR * tipSchape)
					& (sidebandNR <= sidebandCount))
				{
					switch (kernelStyle)
					{
					case 1:
						*wKernel = 1.0 - 1.0 / 16.0 * (float)sidebandNR;
						break;
					case 2:
						//*wKernel = 1.0 / pow(2.0, (float)sidebandNR);
						*wKernel = 1.0 / pow(2.0, (double)sidebandNR);
						break;
					default:
						*wKernel = 1;
						break;
					}
				}
				wKernel++;
			}
		}

		if (saveKernel)
		{
			FileName = FolderName + "Kernel" + FileNameExtension;
			imwrite(FileName, Kernel);
		}

		// Base image definition
		Mat Im;
		int maxXminusOffset = maxX - offsetX; int maxYminusOffset = maxY - offsetY;
		ushort *wIm; //mowing pointer to Im data
		Im = Mat::zeros(maxX, maxY, CV_16U);
		for (int y = offsetY; y <= maxYminusOffset; y += spacingY)
		{
			for (int x = offsetX; x <= maxXminusOffset; x += spacingX)
			{
				wIm = (ushort*)Im.data + y * maxX + x; *wIm = 65535;
			}
		}

		int counttmp = 0;
		for (float a = -90; a <= angleMax; a += angleStep)
		{
			//Rotation of Kernel
			Point rotationCenter = Point(middleKernelX, middleKernelY);
			Mat rotationMatrix = getRotationMatrix2D(rotationCenter, a, 1);
			Mat KernelRotated;
			warpAffine(Kernel, KernelRotated, rotationMatrix, Kernel.size());
			if (saveKernel)
			{
				ostringstream counttmp_convert;
				counttmp_convert << counttmp;  String counttmp_str = counttmp_convert.str();
				ostringstream a_convert; a_convert << a;  string a_str = a_convert.str();
				if (counttmp<10) { counttmp_str = "0" + counttmp_str; } //CHANGE THIS IF counttmp can be >100!!!
				FileName = FolderName + "KernelAngle_" + counttmp_str + "_" + a_str + FileNameExtension;
				imwrite(FileName, KernelRotated * 65535);
			}

			//Filter image
			Mat ImFiltered;
			filter2D(Im, ImFiltered, -1, KernelRotated);
			if (saveResult)
			{
				ostringstream maxX_convert; maxX_convert << maxX;  string maxX_str = maxX_convert.str();
				ostringstream maxY_convert; maxY_convert << maxY;  string maxY_str = maxY_convert.str();
				ostringstream kernelLineLegth_convert; kernelLineLegth_convert << kernelLineLegth;
				string kernelLineLegth_str = kernelLineLegth_convert.str();
				ostringstream sidebandCount_convert; sidebandCount_convert << sidebandCount;
				string sidebandCount_str = sidebandCount_convert.str();
				ostringstream tipSchape_convert; tipSchape_convert << tipSchape;
				string tipSchape_str = tipSchape_convert.str();
				ostringstream kernelStyle_convert; kernelStyle_convert << kernelStyle;
				string kernelStyle_str = kernelStyle_convert.str();
				ostringstream a_convert; a_convert << a;  string a_str = a_convert.str();
				ostringstream counttmp_convert;
				counttmp_convert << counttmp;  String counttmp_str = counttmp_convert.str();
				if (counttmp<10) { counttmp_str = "0" + counttmp_str; } //CHANGE THIS IF counttmp can be >100!!!
				FileName = FolderName + FileNameBase + maxX_str + "x" + maxY_str + "KSBC" + sidebandCount_str + "KL" + kernelLineLegth_str
					+ "TS" + tipSchape_str + "KS" + kernelStyle_str + "KA" + counttmp_str + "_" + a_str + FileNameExtension;
				imwrite(FileName, ImFiltered);
			}
			counttmp++;
		}
	}


	// MY TEMPORARY CODE TO CREATE EIGENTEXTURES
	if (compute_Haralick_feat_yn == -3) {

		int bar_th = 7, bar_lgth = 21, bar_th_gap = 7, bar_lgth_gap = 7;
		float angleStep = 11.25; float angleMax = 90;
		int im_larger_szX = 200, im_larger_szY = 200, im_szX = 100, im_szY = 100;
		bool saveresult = true;

		Mat *im_larger = new Mat();
		(*im_larger) = Mat::zeros(im_larger_szX, im_larger_szY, CV_16U);

		for (int i = 0; i<im_larger_szY; i += bar_th + bar_th_gap) {

			for (int j = 0; j<im_larger_szX; j += bar_lgth + bar_lgth_gap) {

				int k = i + bar_th_gap;
				while ((k - i<bar_th_gap + bar_th)&(k<im_larger_szY)) {
					int l = j + bar_lgth_gap;
					while ((l - j<bar_lgth_gap + bar_lgth)&(l<im_larger_szX)) {
						im_larger->at<ushort>(l, k) = 65535;
						l++;
					}
					k++;
				}

			}

		}

		list <Mat*> im_rotated_list;
		int counttmp = 0;
		for (float a = -90; a <= angleMax; a += angleStep)
		{
			Point center_rot((im_larger_szX - 1) / 2, (im_larger_szY - 1) / 2);
			double scale_rot = 1;
			Mat* rot_mat = new Mat();
			*rot_mat = getRotationMatrix2D(center_rot, a, scale_rot);
			Mat* im_larger_rotated = new Mat();
			warpAffine(*im_larger, *im_larger_rotated, *rot_mat, im_larger->size());
			Mat* im_rotated = new Mat();

			Mat tmp = (*im_larger_rotated)(Rect((im_larger_szY - 1) / 2 - im_szY / 2, (im_larger_szX - 1) / 2 - im_szX / 2, im_szY, im_szX));
			tmp.copyTo(*im_rotated);
			im_rotated_list.push_back(im_rotated);

			if (saveresult)
			{
				//string FolderName = "C:/Users/toniocar/Desktop/Texture_material/texture_VS_project/Input_images/eigentextures_final/";
				string FolderName = "C:/Users/toniocar/Desktop/tempo/";
				ostringstream bar_th_convert; bar_th_convert << bar_th;  string bar_th_str = bar_th_convert.str();
				ostringstream bar_lgth_convert; bar_lgth_convert << bar_lgth;  string bar_lgth_str = bar_lgth_convert.str();
				ostringstream bar_th_gap_convert; bar_th_gap_convert << bar_th_gap;  string bar_th_gap_str = bar_th_gap_convert.str();
				ostringstream bar_lgth_gap_convert; bar_lgth_gap_convert << bar_lgth_gap;  string bar_lgth_gap_str = bar_lgth_gap_convert.str();
				ostringstream a_convert; a_convert << a;  string a_str = a_convert.str();
				ostringstream counttmp_convert;
				counttmp_convert << counttmp;  String counttmp_str = counttmp_convert.str();
				if (counttmp<10) { counttmp_str = "0" + counttmp_str; } //CHANGE THIS IF counttmp can be >100!!!
				string FileName = FolderName + "Eigtxt_bar_th_l_thgap_lgap" + "_" + bar_th_str + "_" + bar_lgth_str
					+ "_" + bar_th_gap_str + "_" + bar_lgth_gap_str + "_ang" + counttmp_str + "_" + a_str + ".tiff";
				imwrite(FileName, *im_rotated);
			}
			counttmp++;
		}

	}

	// TEMPORARY STUFF
	if (compute_Haralick_feat_yn == -4) {
		Mat* im_tmp = new Mat(100, 100, CV_16U);
		*im_tmp = 100;
		(*im_tmp)(Rect(0, 10, 10, 10)) = 10;
		Mat* roi_tmp = new Mat;
		*roi_tmp = (*im_tmp)(Rect(5, 15, 10, 10));
		*roi_tmp = (*roi_tmp) * 2;
		delete roi_tmp;
		imwrite("C:/Users/toniocar/Desktop/prova.tif", *im_tmp);
		//imwrite("C:/Users/toniocar/Desktop/prova_roi.tif",*roi_tmp);
	}

}
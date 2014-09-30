//--f-------------------------------------------------------------------------

#include <opencv2/highgui/highgui.hpp>
#include <opencv2/opencv.hpp>
#include<iostream>
#include<istream>
#include<fstream>
#include<cmath>
#include<string>
#include<list>
#include<windows.h>
#include <ctime>

#include "my_functions.h"

using namespace cv;
using namespace std;

int comput_dist_roi(int*& dist, int*& pix_val, Mat roi_curr, Mat* itext_mod_list, int mod_inset_rows, int mod_inset_cols, 
	Mat*& smaller_text_mod)
{	
	int num_col_roi = roi_curr.cols; int num_row_roi = roi_curr.rows;	
	int num_rows = itext_mod_list->rows; int num_cols = itext_mod_list->cols;	
	int pos_r = (num_rows-mod_inset_rows-1)*rand()/RAND_MAX;	
	int pos_c = (num_cols-mod_inset_cols-1)*rand()/RAND_MAX;	
	Mat tmp_roi = (*itext_mod_list)(Rect(pos_c,pos_r,mod_inset_cols,mod_inset_rows));				
	*smaller_text_mod = tmp_roi.clone(); 
	Mat roi_curr_mask = roi_curr>0; 
	roi_curr_mask.convertTo(roi_curr_mask,roi_curr.type());
	divide(roi_curr_mask, roi_curr_mask, roi_curr_mask);	
	int count = 0;
	for (int i=0; i<mod_inset_cols-(num_col_roi-1); i++) {
		for (int j=0; j<mod_inset_rows-(num_row_roi-1); j++) {			
			Mat roi_iter_curr_tmp = (*smaller_text_mod)(Rect(i,j,num_col_roi,num_row_roi));	
			Mat roi_iter_curr = roi_iter_curr_tmp.clone();			
			multiply(roi_iter_curr, roi_curr_mask, roi_iter_curr);	
			Mat tmp_mat = abs(roi_iter_curr-roi_curr);
			Scalar tmp_Scalar = sum(tmp_mat);
			*(dist+count) = tmp_Scalar[0];
			*(pix_val+count) = roi_iter_curr_tmp.at<ushort>(num_row_roi/2, num_col_roi/2);
			count++;
		}
	}	
	return count;
}

//---------------------------------------------------------------------------

void find_min(int*& min_dist, int*& index_min, int* dist, int sz_dist, double perc, int& sz_index_min)
{
	sz_index_min = perc*sz_dist;
	if (sz_index_min==0) {sz_index_min=1;}	
	min_dist = new int[sz_index_min];
	for (int i=0; i<sz_index_min; i++) {
		*(min_dist+i)=*(dist+i);
	}
	int* sorting_indexes = new int[sz_index_min];		
	bubble_sort_int(min_dist, sorting_indexes, sz_index_min);
	index_min = sorting_indexes;

	for (int i=sz_index_min; i<sz_dist; i++) {
		if (*(dist+i) < *min_dist) {
			int j=1; bool found=false;
			while ( (j<sz_index_min) &&  !found) {
				if ( *(dist+i) > *(min_dist+j) ) {
					found = true;
					for (int k=0; k<j-1; k++ ) 
					{ *(min_dist+k) = *(min_dist+k+1); *(index_min+k) = *(index_min+k+1); }
					*(min_dist+j-1) = *(dist+i);
					*(index_min+j-1) = i;
				}
				j++;
			}
			if (j==sz_index_min && !found) {
				for (int k=0; k<j-1; k++ ) 
				{ *(min_dist+k) = *(min_dist+k+1); *(index_min+k) = *(index_min+k+1); }
				*(min_dist+j-1) = *(dist+i);
				*(index_min+j-1) = i;
			}
		}
	}

	/*min_dist = INT_MAX;
	index_min = -1;
	for (int i=0; i<sz_dist; i++) {
		if (*(dist+i)<min_dist) { min_dist=*(dist+i); index_min=i; }
	}*/
}

//---------------------------------------------------------------------------

void assign_pixel_roi(Mat& roi_curr, int* pix_val, int* index_min, int sz_index_min, Mat* smaller_text_mod, int which_transv)
{	
	int num_rows_roi_curr = roi_curr.rows; int num_cols_roi_curr = roi_curr.cols; 
	
	// CODE TO ASSIGN CENTER PIXEL OF SMALLER_TEXT_MOD (ALG1)
	int pix_val_fin = 0;
	for (int i=0; i<sz_index_min; i++) 
	{ pix_val_fin += *(pix_val+*(index_min+i)); }
	pix_val_fin /=sz_index_min;
	roi_curr.at<ushort>(num_rows_roi_curr/2, num_cols_roi_curr/2) = pix_val_fin;

	// CODE TO ASSIGN THE PART OF SMALLER_TEXT_MOD CORRESPONDING TO ZEROS OF ROI_CURR  (ALG3)
	/*Mat roi_curr_mask = roi_curr==0; 
	roi_curr_mask.convertTo(roi_curr_mask,roi_curr.type());
	divide(roi_curr_mask, roi_curr_mask, roi_curr_mask);	
	int mod_inset_cols = smaller_text_mod->cols; int mod_inset_rows = smaller_text_mod->rows; 
	Mat* roi_smaller_text_mod_fin = new Mat(num_rows_roi_curr,num_cols_roi_curr,roi_curr_mask.type());
	for (int i=0; i<sz_index_min; i++) {
		int col_pos = *(index_min+i)/(mod_inset_rows-(num_rows_roi_curr-1));
		int row_pos = *(index_min+i)%(mod_inset_rows-(num_rows_roi_curr-1));
		Mat roi_smaller_text_mod = (*smaller_text_mod)(Rect(col_pos,row_pos,num_cols_roi_curr,num_rows_roi_curr));
		*roi_smaller_text_mod_fin = *roi_smaller_text_mod_fin+roi_smaller_text_mod;
	}
	*roi_smaller_text_mod_fin = (*roi_smaller_text_mod_fin)/sz_index_min;
	multiply(*roi_smaller_text_mod_fin, roi_curr_mask, *roi_smaller_text_mod_fin);
	roi_curr = roi_curr + *roi_smaller_text_mod_fin;*/

	// CODE TO ASSIGN A SPECIFIC PIXEL OF SMALLER_TEXT_MOD  (ALG4)
	/*int pix_val_fin = 0;
	int mod_inset_cols = smaller_text_mod->cols; int mod_inset_rows = smaller_text_mod->rows; 
	for (int i=0; i<sz_index_min; i++) {
		int col_pos = *(index_min+i)/(mod_inset_rows-(num_rows_roi_curr-1));
		int row_pos = *(index_min+i)%(mod_inset_rows-(num_rows_roi_curr-1));
		Mat roi_smaller_text_mod = (*smaller_text_mod)(Rect(col_pos,row_pos,num_cols_roi_curr,num_rows_roi_curr));		
		if (which_transv==1) {
			pix_val_fin += roi_smaller_text_mod.at<ushort>(num_rows_roi_curr/2, 0);
		} else {
			if (which_transv==2) {
				pix_val_fin += roi_smaller_text_mod.at<ushort>(num_rows_roi_curr/2, num_cols_roi_curr-1);
			} else {
				if (which_transv==3) {
					pix_val_fin += roi_smaller_text_mod.at<ushort>(0, num_cols_roi_curr/2);
				}
				else {				
					pix_val_fin += roi_smaller_text_mod.at<ushort>(num_rows_roi_curr-1, num_cols_roi_curr/2);
				}
			}
		}
	}
	pix_val_fin /=sz_index_min;
	if (which_transv==1) {
		roi_curr.at<ushort>(num_rows_roi_curr/2, 0) = pix_val_fin;
	} else {
		if (which_transv==2) {
			roi_curr.at<ushort>(num_rows_roi_curr/2, num_cols_roi_curr-1) = pix_val_fin;
		} else {
			if (which_transv==3) {
				roi_curr.at<ushort>(0, num_cols_roi_curr/2) = pix_val_fin;
			}
			else {				
				roi_curr.at<ushort>(num_rows_roi_curr-1, num_cols_roi_curr/2) = pix_val_fin;
			}
		}
	}*/
}

//---------------------------------------------------------------------------

void process_mod_border(bool& colpos1_lim, bool& colpos2_lim, bool& rowpos1_lim, bool& rowpos2_lim, int*& pos_col_transv, 
	int*& pos_row_transv, int pad_left, int pad_right, int pad_top, int pad_bottom, Mat*& synth_im_curr_pad, int num_col_roi, 
	int num_row_roi, int mod_inset_rows, int mod_inset_cols, Mat* itext_mod_list, int num_col_synth_im_curr_pad, 
	int num_row_synth_im_curr_pad, double perc_smaller_text_mod, bool colpos1_incr, bool colpos2_incr, bool rowpos1_incr, 
	bool rowpos2_incr, int& count_while)
{	
	if (!colpos1_lim) {
		int end_while=0, start_while=0;
		if (colpos1_incr) { start_while = *(pos_col_transv+2); end_while = *(pos_col_transv+3)+1; } 
		else { end_while = *(pos_col_transv+2)-1; start_while = *(pos_col_transv+3); }
		int i = start_while; bool cond_while=true;
		if (colpos1_incr) {cond_while=i<end_while;} else {cond_while=i>end_while;}

		//for (int i=*(pos_col_transv+2); i<*(pos_col_transv+3)+1; i++) {
		while (cond_while) {
			Mat roi_curr = 
				(*synth_im_curr_pad)(Rect(*(pos_col_transv+0)-(num_col_roi-1)/2, i-(num_row_roi-1)/2, num_col_roi, num_row_roi));
			int sz_dist = (mod_inset_rows-(num_row_roi-1))*(mod_inset_cols-(num_col_roi-1));
			int* dist = new int[sz_dist]; int* pix_val = new int[sz_dist];
			Mat* smaller_text_mod = new Mat(); 
			/*for (int l=0; l<5; l++) {
				for (int m=0; m<5; m++) {
					cout << roi_curr.at<ushort>(l,m) << "\t";
				}
				cout << endl;
			}
			cout << endl;*/
			int sz_dist_out = comput_dist_roi(dist, pix_val, roi_curr, itext_mod_list, mod_inset_rows, mod_inset_cols, smaller_text_mod);
			int* min_dist = NULL; int* index_min = NULL; int sz_index_min;
			find_min(min_dist, index_min, dist, sz_dist, perc_smaller_text_mod, sz_index_min);				
			assign_pixel_roi(roi_curr, pix_val, index_min, sz_index_min, smaller_text_mod, 1);
			delete dist;
			delete smaller_text_mod;			
			if (colpos1_incr) {i++; cond_while=i<end_while;} else {i--; cond_while=i>end_while;}
		}					
		(*(pos_col_transv+0))--; (*(pos_row_transv+0))--;
		//if (*(pos_col_transv+0)-(num_col_roi-1)/2<pad_left) {colpos1_lim=true;}
		if (*(pos_col_transv+0)<pad_left) {colpos1_lim=true;}
	}
	if (!colpos2_lim) {	
		int end_while=0, start_while=0;
		if (colpos2_incr) { start_while = *(pos_col_transv+2); end_while = *(pos_col_transv+3)+1; } 
		else { end_while = *(pos_col_transv+2)-1; start_while = *(pos_col_transv+3); }
		int i = start_while; bool cond_while=true;
		if (colpos2_incr) {cond_while=i<end_while;} else {cond_while=i>end_while;}

		//for (int i=*(pos_col_transv+2); i<*(pos_col_transv+3)+1; i++) {
		while (cond_while) {
			Mat roi_curr = 
				(*synth_im_curr_pad)(Rect(*(pos_col_transv+1)-(num_col_roi-1)/2, i-(num_row_roi-1)/2, num_col_roi, num_row_roi));
			int sz_dist = (mod_inset_rows-(num_row_roi-1))*(mod_inset_cols-(num_col_roi-1));
			int* dist = new int[sz_dist]; int* pix_val = new int[sz_dist];	
			Mat* smaller_text_mod = new Mat();			
			/*for (int l=0; l<5; l++) {
				for (int m=0; m<5; m++) {
					cout << roi_curr.at<ushort>(l,m) << "\t";
				}
				cout << endl;
			}
			cout << endl;*/
			int sz_dist_out = comput_dist_roi(dist, pix_val, roi_curr, itext_mod_list, mod_inset_rows, mod_inset_cols, smaller_text_mod);
			int* min_dist = NULL; int* index_min = NULL; int sz_index_min;
			find_min(min_dist, index_min, dist, sz_dist, perc_smaller_text_mod, sz_index_min);						
			assign_pixel_roi(roi_curr, pix_val, index_min, sz_index_min, smaller_text_mod, 2);
			delete dist;
			delete smaller_text_mod;
			if (colpos2_incr) {i++; cond_while=i<end_while;} else {i--; cond_while=i>end_while;}
		}					
		(*(pos_col_transv+1))++; (*(pos_row_transv+1))++;
		//if (*(pos_col_transv+1)+(num_col_roi-1)/2>num_col_synth_im_curr_pad-1-pad_right) {colpos2_lim=true;}	
		if (*(pos_col_transv+1)>num_col_synth_im_curr_pad-1-pad_right) {colpos2_lim=true;}
	}
	if (!rowpos1_lim) {	
		int end_while=0, start_while=0;
		if (rowpos1_incr) { start_while = *(pos_row_transv+0); end_while = *(pos_row_transv+1)+1; } 
		else { end_while = *(pos_row_transv+0)-1; start_while = *(pos_row_transv+1); }
		int i = start_while; bool cond_while=true;
		if (rowpos1_incr) {cond_while=i<end_while;} else {cond_while=i>end_while;}

		//for (int i=*(pos_row_transv+0); i<*(pos_row_transv+1)+1; i++) {
		while (cond_while) {
			Mat roi_curr = 
				(*synth_im_curr_pad)(Rect(i-(num_col_roi-1)/2, *(pos_row_transv+2)-(num_row_roi-1)/2, num_col_roi, num_row_roi));
			int sz_dist = (mod_inset_rows-(num_row_roi-1))*(mod_inset_cols-(num_col_roi-1));
			int* dist = new int[sz_dist]; int* pix_val = new int[sz_dist];	
			Mat* smaller_text_mod = new Mat();			
			/*for (int l=0; l<5; l++) {
				for (int m=0; m<5; m++) {
					cout << roi_curr.at<ushort>(l,m) << "\t";
				}
				cout << endl;
			}
			cout << endl;*/
			int sz_dist_out = comput_dist_roi(dist, pix_val, roi_curr, itext_mod_list, mod_inset_rows, mod_inset_cols, smaller_text_mod);
			int* min_dist = NULL; int* index_min = NULL; int sz_index_min;
			find_min(min_dist, index_min, dist, sz_dist, perc_smaller_text_mod, sz_index_min);						
			assign_pixel_roi(roi_curr, pix_val, index_min, sz_index_min, smaller_text_mod, 3);
			delete dist;
			delete smaller_text_mod;
			if (rowpos1_incr) {i++; cond_while=i<end_while;} else {i--; cond_while=i>end_while;}
		}					
		(*(pos_row_transv+2))--; (*(pos_col_transv+2))--;
		//if (*(pos_row_transv+2)-(num_row_roi-1)/2<pad_top) {rowpos1_lim=true;}			
		if (*(pos_row_transv+2)<pad_top) {rowpos1_lim=true;}
	}
	if (!rowpos2_lim) {
		int end_while=0, start_while=0;
		if (rowpos2_incr) { start_while = *(pos_row_transv+0); end_while = *(pos_row_transv+1)+1; } 
		else { end_while = *(pos_row_transv+0)-1; start_while = *(pos_row_transv+1); }
		int i = start_while; bool cond_while=true;
		if (rowpos2_incr) {cond_while=i<end_while;} else {cond_while=i>end_while;}

		//for (int i=*(pos_row_transv+0); i<*(pos_row_transv+1)+1; i++) {
		while (cond_while) {
			Mat roi_curr = 
				(*synth_im_curr_pad)(Rect(i-(num_col_roi-1)/2, *(pos_row_transv+3)-(num_row_roi-1)/2, num_col_roi, num_row_roi));
			int sz_dist = (mod_inset_rows-(num_row_roi-1))*(mod_inset_cols-(num_col_roi-1));
			int* dist = new int[sz_dist]; int* pix_val = new int[sz_dist];	
			Mat* smaller_text_mod = new Mat();			
			/*for (int l=0; l<5; l++) {
				for (int m=0; m<5; m++) {
					cout << roi_curr.at<ushort>(l,m) << "\t";
				}
				cout << endl;
			}
			cout << endl;*/
			int sz_dist_out = comput_dist_roi(dist, pix_val, roi_curr, itext_mod_list, mod_inset_rows, mod_inset_cols, smaller_text_mod);
			int* min_dist = NULL; int* index_min = NULL; int sz_index_min;
			find_min(min_dist, index_min, dist, sz_dist, perc_smaller_text_mod, sz_index_min);						
			assign_pixel_roi(roi_curr, pix_val, index_min, sz_index_min, smaller_text_mod, 4);
			delete dist;
			delete smaller_text_mod;
			if (rowpos2_incr) {i++; cond_while=i<end_while;} else {i--; cond_while=i>end_while;}
		}					
		(*(pos_row_transv+3))++; (*(pos_col_transv+3))++;	
		//if (*(pos_row_transv+3)+(num_row_roi-1)/2>num_row_synth_im_curr_pad-1-pad_bottom) {rowpos2_lim=true;}
		if (*(pos_row_transv+3)>num_row_synth_im_curr_pad-1-pad_bottom) {rowpos2_lim=true;}
	}
	count_while++;
	//cout << count_while << endl;
	if (count_while%10==0) {				
		cout << *(pos_col_transv+0) << "\t" << *(pos_col_transv+1) << "\t" << *(pos_col_transv+2) << "\t" 
			<< *(pos_col_transv+3) << endl;	
		cout << *(pos_row_transv+0) << "\t" << *(pos_row_transv+1) << "\t" << *(pos_row_transv+2) << "\t" 
			<< *(pos_row_transv+3) << endl << endl;	
	}
}

//---------------------------------------------------------------------------

void bubble_sort_int(int*& min_dist, int*& sorting_indexes, int sz_index_min)
{
	for (int i=0; i<sz_index_min; i++) { *(sorting_indexes+i)=i; }
	int curr_sz = sz_index_min;
	while (curr_sz>0) {
		int new_curr_sz = 0;
		for (int i=1; i<curr_sz; i++) {
			if ( *(min_dist+i-1) < *(min_dist+i) ) {
				// swap values
				int tmp = *(min_dist+i-1);
				*(min_dist+i-1) = *(min_dist+i);
				*(min_dist+i) = tmp;
				// swap indexes
				tmp = *(sorting_indexes+i-1);
				*(sorting_indexes+i-1)=*(sorting_indexes+i);
				*(sorting_indexes+i)=tmp;
				new_curr_sz = i;
			}
		}
		curr_sz = new_curr_sz;
	}
}

//---------------------------------------------------------------------------
Mat* get_co_occurrence_mat(int offset, double ang_text, int num_bins, double max_val, double min_val, Mat* im_inp)
{
	// Redefine im_inp elements based on num_bins, max_val and min_val
	Point tmp1, tmp2; 
	if (max_val<0) { minMaxLoc(*im_inp, &min_val, &max_val, &tmp1, &tmp2); }
	double dividend = (max_val - min_val)/num_bins;
	//cout << "dividend: " << dividend << endl;
	for (int i=0; i<im_inp->rows; i++) {
		for (int j=0; j<im_inp->cols;j++) {
			// image type will determing below line: generalize this code!!!
			//cout << "original value: " << im_inp->at<ushort>(i,j) << endl;
			int new_val = max ( (int) ( im_inp->at<ushort>(i,j) - min_val ) / dividend + 1, 1);			
			new_val = min(num_bins, new_val);
			//cout << "new value: " << new_val << endl;
			im_inp->at<ushort>(i,j) = new_val;			
		}
	}

	/*double tmp1_d, tmp2_d;
	minMaxLoc(*im_inp, &tmp1_d, &tmp2_d, &tmp1, &tmp2);*/

	// Define co_occ_mat
	Mat* co_occ_mat = new Mat();
	*co_occ_mat = Mat::zeros(num_bins, num_bins, CV_32S);
	
	// Build mask based on offset and ang_text
	Mat* kernel1 = new Mat(1+offset*2,1+offset*2,CV_32F,0.); Mat* kernel2 = new Mat(1+offset*2,1+offset*2,CV_32F,0.);
	kernel1->at<float>(0,offset) = 1; kernel2->at<float>(offset*2,offset) = 1;
	Point center_rot(offset,offset);				
	double scale_rot = 1;
	Mat rot_mat = getRotationMatrix2D(center_rot, ang_text, scale_rot);
	warpAffine(*kernel1, *kernel1, rot_mat, kernel1->size(), INTER_CUBIC);
	warpAffine(*kernel2, *kernel2, rot_mat, kernel1->size(), INTER_CUBIC);
	
	// Pad im_inp			
	Mat* im_inp_pad = new Mat();	
	copyMakeBorder(*im_inp, *im_inp_pad, offset, offset, offset, offset, BORDER_CONSTANT, 0);
	
	// build co_occ_mat
	for (int i=offset; i<im_inp_pad->rows-offset; i++) {
		for (int j=offset; j<im_inp_pad->cols-offset;j++) {
			
			int ind1 = im_inp_pad->at<ushort>(i,j);
			
			Mat patch_curr = (*im_inp_pad)(Rect(j-offset,i-offset,1+offset*2,1+offset*2));
			Mat* tmp_mat = new Mat();			
			patch_curr.convertTo(patch_curr,kernel1->type());
			multiply(patch_curr, *kernel1, *tmp_mat);
			/*for (int l=0; l<5; l++) {
				for (int m=0; m<5; m++) {
					cout << patch_curr.at<float>(l,m) << "\t";
				}
				cout << endl;
			}
			cout << endl;
			for (int l=0; l<5; l++) {
				for (int m=0; m<5; m++) {
					cout << kernel1->at<float>(l,m) << "\t";
				}
				cout << endl;
			}
			cout << endl;
			for (int l=0; l<5; l++) {
				for (int m=0; m<5; m++) {
					cout << tmp_mat->at<float>(l,m) << "\t";
				}
				cout << endl;
			}
			cout << endl;*/
			Scalar tmp_Scalar = sum(*tmp_mat);
			float tmp_float = tmp_Scalar[0];			
			int ind2 = floor(tmp_float+0.5);
			ind2 = min(ind2, num_bins);
			//int ind2 = tmp_Scalar[0];
			if (ind2>0) {
				co_occ_mat->at<int>(ind1-1,ind2-1)++;
			}
			/*for (int l=0; l<num_bins; l++) {
				for (int m=0; m<num_bins; m++) {
					cout << co_occ_mat->at<int>(l,m) << "\t";
				}
				cout << endl;
			}
			cout << endl;*/

			patch_curr = (*im_inp_pad)(Rect(j-offset,i-offset,1+offset*2,1+offset*2));
			patch_curr.convertTo(patch_curr,kernel2->type());
			multiply(patch_curr, *kernel2, *tmp_mat);
			/*for (int l=0; l<3; l++) {
				for (int m=0; m<3; m++) {
					cout << patch_curr.at<float>(l,m) << "\t";
				}
				cout << endl;
			}
			cout << endl;
			for (int l=0; l<3; l++) {
				for (int m=0; m<3; m++) {
					cout << kernel2->at<float>(l,m) << "\t";
				}
				cout << endl;
			}
			cout << endl;
			for (int l=0; l<3; l++) {
				for (int m=0; m<3; m++) {
					cout << tmp_mat->at<float>(l,m) << "\t";
				}
				cout << endl;
			}
			cout << endl;*/
			tmp_Scalar = sum(*tmp_mat);
			tmp_float = tmp_Scalar[0];			
			ind2 = floor(tmp_float+0.5);
			ind2 = min(ind2, num_bins);
			//ind2 = tmp_Scalar[0];
			if (ind2>0) {
				co_occ_mat->at<int>(ind1-1,ind2-1)++;
			}
			/*for (int l=0; l<num_bins; l++) {
				for (int m=0; m<num_bins; m++) {
					cout << co_occ_mat->at<int>(l,m) << "\t";
				}
				cout << endl;
			}
			cout << endl;*/
			delete tmp_mat;
		}
	}
	delete kernel1; delete kernel2; 
	return co_occ_mat;
}
//---------------------------------------------------------------------------
void get_co_occurrence_mat_props(Mat* co_occ_mat, double& contrast, double& energy, double& homogeneity, double& correlation)
{
	// normalize co_occ_mat
	Scalar tot_sum_Sc = sum(*co_occ_mat);
	int tot_sum = tot_sum_Sc[0];
	Mat* co_occ_mat_norm = new Mat(co_occ_mat->size(), CV_32F);
	Mat co_occ_mat_tmp;
	co_occ_mat->convertTo(co_occ_mat_tmp, CV_32F);
	*co_occ_mat_norm = co_occ_mat_tmp / tot_sum;

	// compute contrast, energy, homogeneity, mu_i, mu_j
	contrast = 0; energy = 0; homogeneity = 0;
	double mu_i = 0, mu_j = 0;

	for (int i=0; i<co_occ_mat->rows; i++) {
		for (int j=0; j<co_occ_mat->cols;j++) {
			contrast += (i-j)*(i-j)*co_occ_mat_norm->at<float>(i,j);
			energy += co_occ_mat_norm->at<float>(i,j)*co_occ_mat_norm->at<float>(i,j);
			homogeneity += co_occ_mat_norm->at<float>(i,j)/(1+abs(i-j));
			mu_i += (i+1)*co_occ_mat_norm->at<float>(i,j);
			mu_j += (j+1)*co_occ_mat_norm->at<float>(i,j);
		}
	}

	// compute sigma_i, sigma_j
	double sigma_i = 0, sigma_j = 0;
	for (int i=0; i<co_occ_mat->rows; i++) {
		for (int j=0; j<co_occ_mat->cols;j++) {
			sigma_i += (i+1 - mu_i)*(i+1 - mu_i)*co_occ_mat_norm->at<float>(i,j);
			sigma_j += (j+1 - mu_j)*(j+1 - mu_j)*co_occ_mat_norm->at<float>(i,j);
		}
	}
	sigma_i = sqrt(sigma_i);
	sigma_j = sqrt(sigma_j);

	// compute correlation
	correlation = 0;
	for (int i=0; i<co_occ_mat->rows; i++) {
		for (int j=0; j<co_occ_mat->cols;j++) {
			correlation += (i+1 - mu_i)*(j+1 - mu_j)*co_occ_mat_norm->at<float>(i,j);
		}
	}
	correlation /= sigma_i*sigma_j;
	delete co_occ_mat_norm;
}


//---------------------------------------------------------------------------

void get_min_from_vector(double* vector, int vector_sz, int& pos, double& val)
{
	pos = 0; val = *vector;
	for (int i=1; i<vector_sz; i++) {
		if (*(vector+i)<val) {
			val=*(vector+i); pos = i;
		}
	}
}

//---------------------------------------------------------------------------

void get_min_from_vector(int* vector, int vector_sz, int& pos, int& val, bool& multiple_occ)
{
	multiple_occ = false;
	pos = 0; val = *vector;
	for (int i=1; i<vector_sz; i++) {
		if (*(vector+i)<val) {
			val=*(vector+i); pos = i;
		}
	}
	for (int i=1; i<vector_sz; i++) {
		if ( (*(vector+i)==val) && (i!=pos) ) {
			multiple_occ = true;
			break;
		}
	}
}

//---------------------------------------------------------------------------

void get_max_from_vector(double* vector, int vector_sz, int& pos, double& val)
{
	pos = 0; val = *vector;
	for (int i=1; i<vector_sz; i++) {
		if (*(vector+i)>val) {
			val=*(vector+i); pos = i;
		}
	}
}

//---------------------------------------------------------------------------

void get_max_from_vector(int* vector, int vector_sz, int& pos, int& val, bool& multiple_occ)
{
	multiple_occ = false;
	pos = 0; val = *vector;
	for (int i=1; i<vector_sz; i++) {
		if (*(vector+i)>val) {
			val=*(vector+i); pos = i;
		}
	}
	for (int i=1; i<vector_sz; i++) {
		if ( (*(vector+i)==val) && (i!=pos) ) {
			multiple_occ = true;
			break;
		}
	}
}

//---------------------------------------------------------------------------

void get_mean_std_from_vector(double* vector, int vector_sz, double& mean, double& std)
{
	mean = 0; for (int i=0; i<vector_sz; i++) { mean += *(vector+i); }
	mean /= vector_sz;
	std = 0; for (int i=0; i<vector_sz; i++) { std += ( *(vector+i) - mean ) * ( *(vector+i) - mean ); }
	std /= vector_sz; std = sqrt(std);
}

//---------------------------------------------------------------------------

void erase_el_from_vector(double* vector_in, double*& vector_out, int& vector_sz, int pos)
{
	vector_out = new double[vector_sz-1];
	int curr_pos = 0;
	for (int i=0; i<pos; i++) {
		*(vector_out+curr_pos)=*(vector_in+i);
		curr_pos++;
	}
	for (int i=pos+1; i<vector_sz; i++) {
		*(vector_out+curr_pos)=*(vector_in+i);
		curr_pos++;
	}
	vector_sz -= 1;
}

void erase_el_from_vector(int* vector_in, int*& vector_out, int& vector_sz, int pos)
{
	vector_out = new int[vector_sz-1];
	int curr_pos = 0;
	for (int i=0; i<pos; i++) {
		*(vector_out+curr_pos)=*(vector_in+i);
		curr_pos++;
	}
	for (int i=pos+1; i<vector_sz; i++) {
		*(vector_out+curr_pos)=*(vector_in+i);
		curr_pos++;
	}
	vector_sz -= 1;
}































//int num_rows_roi_curr = roi_curr.rows; int num_cols_roi_curr = roi_curr.cols; 
	//// CODE TO ASSIGN CENTER PIXEL OF SMALLER_TEXT_MOD
	//roi_curr.at<ushort>(num_rows_roi_curr/2, num_cols_roi_curr/2) = *(pix_val+index_min);

	//// CODE TO ASSIGN THE WHOLE SMALLER_TEXT_MOD
	///*int mod_inset_cols = smaller_text_mod->cols; int mod_inset_rows = smaller_text_mod->rows; 
	//int col_pos = index_min/(mod_inset_rows-(num_rows_roi_curr-1));
	//int row_pos = index_min%(mod_inset_rows-(num_rows_roi_curr-1));
	//Mat roi_smaller_text_mod = (*smaller_text_mod)(Rect(col_pos,row_pos,num_cols_roi_curr,num_rows_roi_curr));
	//roi_smaller_text_mod.copyTo(roi_curr);*/

	//// CODE TO ASSIGN THE PART OF SMALLER_TEXT_MOD CORRESPONDING TO ZEROS OF ROI_CURR
	///*Mat roi_curr_mask = roi_curr==0; 
	//roi_curr_mask.convertTo(roi_curr_mask,roi_curr.type());
	//divide(roi_curr_mask, roi_curr_mask, roi_curr_mask);	
	//int mod_inset_cols = smaller_text_mod->cols; int mod_inset_rows = smaller_text_mod->rows; 
	//int col_pos = index_min/(mod_inset_rows-(num_rows_roi_curr-1));
	//int row_pos = index_min%(mod_inset_rows-(num_rows_roi_curr-1));
	//Mat roi_smaller_text_mod = (*smaller_text_mod)(Rect(col_pos,row_pos,num_cols_roi_curr,num_rows_roi_curr));
	//multiply(roi_smaller_text_mod, roi_curr_mask, roi_smaller_text_mod);
	//roi_curr = roi_curr + roi_smaller_text_mod;*/

	//// CODE TO ASSIGN A SPECIFIC PIXEL OF SMALLER_TEXT_MOD		
	///*int mod_inset_cols = smaller_text_mod->cols; int mod_inset_rows = smaller_text_mod->rows; 
	//int col_pos = index_min/(mod_inset_rows-(num_rows_roi_curr-1));
	//int row_pos = index_min%(mod_inset_rows-(num_rows_roi_curr-1));
	//Mat roi_smaller_text_mod = (*smaller_text_mod)(Rect(col_pos,row_pos,num_cols_roi_curr,num_rows_roi_curr));
	//if (which_transv==1) {
	//	roi_curr.at<ushort>(num_rows_roi_curr/2, 0) = roi_smaller_text_mod.at<ushort>(num_rows_roi_curr/2, 0);
	//} else {
	//	if (which_transv==2) {
	//		roi_curr.at<ushort>(num_rows_roi_curr/2, num_cols_roi_curr-1) = 
	//			roi_smaller_text_mod.at<ushort>(num_rows_roi_curr/2, num_cols_roi_curr-1);
	//	} else {
	//		if (which_transv==3) {
	//			roi_curr.at<ushort>(0, num_cols_roi_curr/2) = roi_smaller_text_mod.at<ushort>(0, num_cols_roi_curr/2);
	//		}
	//		else {				
	//			roi_curr.at<ushort>(num_rows_roi_curr-1, num_cols_roi_curr/2) = 
	//				roi_smaller_text_mod.at<ushort>(num_rows_roi_curr-1, num_cols_roi_curr/2);
	//		}
	//	}
	//}*/
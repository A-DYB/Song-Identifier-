/*
 * match.c
 *
 * Author: Adrian Dybka
 */
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>

#include "match.h"

typedef struct Scores{
	int max_score;
	char name[30];
} Score;


Score match(double clip[][num_bands], int clip_wind, double song1[][num_bands],int num_wind1, double song2[][num_bands],int num_wind2,double song3[][num_bands],int num_wind3){
	int match_count=0;

	//return this if two or more songs have the same score
	Score none;
	strcpy(none.name , "Try using a longer sound clip");
	none.max_score=0;

	Score sad_score;
	strcpy(sad_score.name , "sad");
	sad_score.max_score=0;
	Score buddy_score;
	strcpy(buddy_score.name , "buddy");
	buddy_score.max_score=0;
	Score epic_score;
	strcpy(epic_score.name , "epic");
	epic_score.max_score=0;

		//for loop for the whole song
		for(int i =0;i<num_wind1;i++){
			for(int j =0;j<clip_wind;j++){
				for(int k=0;k<6;k++){
				//loop through the clip

					if(i+clip_wind-1<num_wind1){
						if( abs( clip[j][k] ) > 0.0001 && abs( song1[i][k] ) > 0.0001){
							if( abs( clip[j][k] - song1[i][k] )<0.0001 ){
								match_count++;
							}
						}
					}
				}
			}
			if(match_count>sad_score.max_score)
				sad_score.max_score = match_count;

			match_count =0;
		}

		////////

		for(int i =0;i<num_wind2;i++){
			for(int j =0;j<clip_wind;j++){
				for(int k=0;k<6;k++){
				//loop through the clip

					if(i+clip_wind-1<num_wind2){
						if( abs( clip[j][k] ) > 0.0001 && abs( song2[i][k] ) > 0.0001){
							if( abs( clip[j][k] - song2[i][k] )<0.0001 ){
								match_count++;
							}
						}
					}
				}
			}
			if(match_count>buddy_score.max_score)
				buddy_score.max_score = match_count;

			match_count =0;
		}

		////////

		for(int i =0;i<num_wind3;i++){
			for(int j =0;j<clip_wind;j++){
				for(int k=0;k<6;k++){
				//loop through the clip

					if(i+clip_wind-1<num_wind3){
						if( abs( clip[j][k] ) > 0.0001 && abs( song3[i][k] ) > 0.0001){
							if( abs( clip[j][k] - song3[i][k] )<0.0001 ){
								match_count++;
							}
						}
					}
				}
			}
			if(match_count>epic_score.max_score)
				epic_score.max_score = match_count;

			match_count =0;
		}

		if(optimization ==0){
			printf("Sad Song score: %d\n", sad_score.max_score);
			printf("Buddy song score: %d\n", buddy_score.max_score);
			printf("Epic song score: %d\n", epic_score.max_score);

		}

		if(sad_score.max_score>epic_score.max_score && sad_score.max_score>buddy_score.max_score){
			return sad_score;
		}
		else if(buddy_score.max_score>sad_score.max_score && buddy_score.max_score>epic_score.max_score){
			return buddy_score;
		}
		else if(epic_score.max_score>sad_score.max_score && epic_score.max_score>buddy_score.max_score){
			return epic_score;
		}

		return none;
}

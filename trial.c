#include <stdio.h>
#include <ctype.h>
#include <error.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>
#include <unistd.h>
#include <fcntl.h>

#define SIZE_OF_STRUCT 72 	// max children can be 72
#define STRUCT_NAME_SIZE 100
#define STRUCT_DESCRIPTION_SIZE 2048

struct node {
	int layer;
	char name[STRUCT_NAME_SIZE];
	char description[STRUCT_DESCRIPTION_SIZE];
	// int num_children; 	// bonus if time permits
};
struct node res[SIZE_OF_STRUCT];

char str[2000000000];

#define MAXSIZE 2000000000

int MP4Parser(char *s); //struct node *keywordlink);
int keywordcaller(char *s, char *t, int index);
int ftype(char *t, int ind);
int moov(char *t, int ind);
int mvhd(char *t, int ind);
int trak(char *t, int ind);
int tkhd(char *t, int ind);
int tref(char *t, int ind);
int edts(char *t, int ind);
int elst(char *t, int ind);
int mdia(char *t, int ind);
int mdhd(char *t, int ind);
int hdlr(char *t, int ind);
int minf(char *t, int ind);
int vmhd(char *t, int ind);
int smhd(char *t, int ind);
int hmhd(char *t, int ind);
int nmhd(char *t, int ind);
int dinf(char *t, int ind);
int dref(char *t, int ind);
int stbl(char *t, int ind);
int stsd(char *t, int ind);
int stts(char *t, int ind);
int stsc(char *t, int ind);
int stco(char *t, int ind);
int mvex(char *t, int ind);
int trex(char *t, int ind);
int moof(char *t, int ind);
int mfhd(char *t, int ind);
int traf(char *t, int ind);
int tfhd(char *t, int ind);
int mfra(char *t, int ind);
int mfro(char *t, int ind);
void print_struct(struct node *n);
void itob(int n, char s[], int base);


int strind = 0;
int holder = 0;

int main()
{
	FILE *fp;
	int c;

	const char *filename = "Sample2.mp4";
	fp = fopen(filename,"rb");
	if (fp == NULL) {
	    perror("Error opening file");
	    return 0;
	}

	int i = 0;
	char tmpr[2];
	do {
		c = fgetc(fp);
		if( feof(fp) ) {
			break ;
		}
		// option 1
		itob(c,tmpr,16);
		if(tmpr[1] == '\0'){
			str[i] = '0';
			str[i+1] = tmpr[0];
		}
		else{
			str[i] = tmpr[1];
			str[i+1] = tmpr[0];
		}
		// option 1 end

		// option 2
		// sprintf(line+i,"%02x",c);	// works but slow
		// option 2 end

		i+=2;
	} while(1 && i < 2000000000);
	if(i == 2000000000-1){
		printf("Error input too large\n");
		return 0;
	}
	// printf("%s\n",str);
	long long j = 0, k = 0, tmper = 0, infolen = 0, count = 1;
	int num, hld, letrflg = 0, remain = 0;
	long long len = (long long)strlen(str);
	char str2[10000], info[1000000] = "", tmp[10000];
	while (j < len) {
		//printf("let me guaess not getting in\n");
		while (count < 4) {
			if (str[j] != '0') {
				str2[k++] = str[j++];
				str2[k++] = str[j++];
				str2[k] = '\0';
				num = (long)strtol(str2, NULL, 16);
				k=0;
				if (isalpha(num)) {
					sprintf(tmp, "%c", num);
					strcat(info, tmp);
					count++;
				} else {
					count = 0;
					continue;
				}
			} else if (remain == 0) {
				j = j + 8;
				count = 0;
				//printf("j: %lld\n", j);
				break;
			} else if (remain != 0) {
				j = j + remain;
				remain = 8;
				//printf("j: %lld\n", j);
				count = 0;
				break;
			}
		}
		hld = j;
		j = keywordcaller(info, str, j);
		if (j == -1) {
			print_struct(res);
			return 0;
		}
		if (j > hld)
			remain = j % 8;
		else if (count == 4)
			remain = 0;
		info[0] = '\0';
		count = 0;
	}
	return 0;
}

void itob(int n, char s[], int base)
{
	int i, q;
	i = 0;
	do {
		q = n % base;
	 	if (q > 9)
			s[i++] = (n % base )+ 55;
		else
			s[i++] = n % base + '0';

	} while ((n /= base) > 0);
	s[i] = '\0';
}

void print_struct(struct node *n)
{
	for (int i = 0; i < SIZE_OF_STRUCT && strcmp(n[i].name, "") != 0; i++) {
		printf("Name is :%s\n", n[i].name);
		printf("%s\n",n[i].description );
	}
}

int keywordcaller(char *s, char *t, int ind)
{
	int ret = 0;
	if (strcmp(s, "ftyp") == 0) {
		ret = ftype(t, ind);
	} else if (strcmp(s, "moov") == 0) {
		ret = moov(t, ind);
	} else if (strcmp(s, "mvhd") == 0) {
		ret = mvhd(t, ind);
	} else if (strcmp(s, "trak") == 0) {
		ret = trak(t, ind);
	} else if (strcmp(s, "tkhd") == 0) {
		ret = tkhd(t, ind);
	} else if (strcmp(s, "tref") == 0) {
		ret = tref(t, ind);
	} else if (strcmp(s, "edts") == 0) {
		ret = edts(t, ind);
	} else if (strcmp(s, "elst") == 0) {
		ret = elst(t, ind);
	} else if (strcmp(s, "mdia") == 0) {
		ret = mdia(t, ind);
	} else if (strcmp(s, "mdhd") == 0) {
		ret = mdhd(t, ind);
	} else if (strcmp(s, "hdlr") == 0) {
		ret = hdlr(t, ind);
	} else if (strcmp(s, "minf") == 0) {
		ret = minf(t, ind);
	} else if (strcmp(s, "vmhd") == 0) {
		ret = vmhd(t, ind);
	} else if (strcmp(s, "smhd") == 0) {
		ret = smhd(t, ind);
	} else if (strcmp(s, "hmhd") == 0) {
		ret = hmhd(t, ind);
	} else if (strcmp(s, "nmhd") == 0) {
		ret = nmhd(t, ind);
	} else if (strcmp(s, "dinf") == 0) {
		ret = dinf(t, ind);
	} else if (strcmp(s, "dref") == 0) {
		ret = dref(t, ind);
	} else if (strcmp(s, "stbl") == 0) {
		ret = stbl(t, ind);
	} else if (strcmp(s, "stsd") == 0) {
		ret = stsd(t, ind);
	} else if (strcmp(s, "stts") == 0) {
		ret = stts(t, ind);
	} else if (strcmp(s, "stsc") == 0) {
		ret = stsc(t, ind);
	} else if (strcmp(s, "stco") == 0) {
		ret = stco(t, ind);
	} else if (strcmp(s, "mvex") == 0) {
		ret = mvex(t, ind);
	} else if (strcmp(s, "trex") == 0) {
		ret = trex(t, ind);
	} else if (strcmp(s, "moof") == 0) {
		ret = moof(t, ind);
	} else if (strcmp(s, "mfhd") == 0) {
		ret = mfhd(t, ind);
	} else if (strcmp(s, "traf") == 0) {
		ret = traf(t, ind);
	} else if (strcmp(s, "tfhd") == 0) {
		ret = tfhd(t, ind);
	} else if (strcmp(s, "mfra") == 0) {
		ret = mfra(t, ind);
	} else if (strcmp(s, "mfro") == 0) {
		ret = mfro(t, ind);
	} else if (strcmp(s, "mdat") == 0) {
		printf("mdat found...\n\nPrinting metadata...\n\n");
		return -1;
	} else {
		ret = ind;
	}
	return ret;
}

int ftype(char *t, int ind)
{
	//layer 0
	char info[1000], tmp[100], tmp2[100], tmp3[100];
	strcpy(info, " ftype");
	strcpy(res[strind].name, info);
	strcpy(info, "Major Brand: ");
	int tmpnum, i = 0, j = 0, k = 0, count = 1, flag = 0;
	while (j < 8) {
		tmp[j++] = t[ind++];
	}
	tmp[j] = '\0';
	for (j = 0; j < strlen(tmp)-1; ) {
		tmp2[k++] = tmp[j++];
		tmp2[k++] = tmp[j++];
		tmpnum = (int)strtol(tmp2, NULL, 16);
		sprintf(tmp3, "%c", tmpnum);
		strcat(info, tmp3);
		k=0;
	}
	strcat(info, "\nCompatible Brands: \n");
	ind = ind + 8;
	while (ind < strlen(t)-1) {
		while (count < 5) {
			if (t[ind] != '0') {
				tmp2[k++] = t[ind++];
				tmp2[k++] = t[ind++];
				tmpnum = (int)strtol(tmp2, NULL, 16);
				sprintf(tmp3, "%c", tmpnum);
				strcat(info, tmp3);
				k = 0;
				count++;
			} else {
				flag = 1;
				break;
			}
		}
		strcat(info, "\n");
		count = 0;
		if (flag == 1)
			break;
	}
	//printf("%s\n", info);
	strcpy(res[strind].description, info);
	res[strind].layer = 0;
	strind++;
	// printf("ind: %d\n", ind);
	holder = ind;
	return ind + 8;

}

int moov(char *t, int ind)
{
	//layer 0
	char info[100];
	strcpy(info, " moov");
	//printf("%s\n", info);
	strcpy(res[strind].name, info);
	strcpy(res[strind].description, "");
	res[strind].layer = 0;
	strind++;
	// printf("ind: %d\n", ind);
	holder = ind;
	return ind + 8;
}

int mvhd(char *t, int ind)
{
	//layer 1
	char info[1000], tmp[1000];
	int i = 0, j = 0;
	long tmpnum;
	if (strcmp(res[strind-1].name, " moov") != 0) {
		holder += 2;
		return holder;
	}
	strcpy(info, " mvhd");
	strcpy(res[strind].name, info);
	strcpy(info, "Box Version: ");
	if (t[ind+4] == 1) {
		strcat(info, "1\n");
		ind = ind + 8;
		i = ind;
		while (ind < i + 16)
			tmp[j++] = t[ind++];
		tmp[j] = '\0';
		tmpnum = (long)strtol(tmp, NULL, 16);
		sprintf(tmp, "Creation Time: %ld\n", tmpnum);
		strcat(info, tmp);
		i = i + 16;
		j = 0;
		while (ind < i + 16)
			tmp[j++] = t[ind++];
		tmp[j] = '\0';
		tmpnum = (long)strtol(tmp, NULL, 16);
		sprintf(tmp, "Modification Time: %ld\n", tmpnum);
		strcat(info, tmp);
		i = i + 16;
		j = 0;
		while (ind < i + 8)
			tmp[j++] = t[ind++];
		tmp[j] = '\0';
		tmpnum = (long)strtol(tmp, NULL, 16);
		sprintf(tmp, "Timescale: %ld\n", tmpnum);
		strcat(info, tmp);
		i = i + 8;
		j = 0;
		while (ind < i + 16)
			tmp[j++] = t[ind++];
		tmp[j] = '\0';
		tmpnum = (long)strtol(tmp, NULL, 16);
		sprintf(tmp, "Duration: %ld\n", tmpnum);
		strcat(info, tmp);
	} else {
		strcat(info, "0\n");
		ind = ind + 8;
		i = ind;
		j = 0;
		while (ind < i + 8)//16
			tmp[j++] = t[ind++];
		tmp[j] = '\0';
		tmpnum = (long)strtol(tmp, NULL, 16);
		sprintf(tmp, "Creation Time: %ld\n", tmpnum);
		strcat(info, tmp);
		i = i + 8;
		j = 0;
		while (ind < i + 8) //24
			tmp[j++] = t[ind++];
		tmp[j] = '\0';
		tmpnum = (long)strtol(tmp, NULL, 16);
		sprintf(tmp, "Modification Time: %ld\n", tmpnum);
		strcat(info, tmp);
		i = i + 8;
		j = 0;
		while (ind < i + 8) //32
			tmp[j++] = t[ind++];
		tmp[j] = '\0';
		tmpnum = (long)strtol(tmp, NULL, 16);
		sprintf(tmp, "Timescale: %ld\n", tmpnum);
		strcat(info, tmp);
		i = i + 8;
		j = 0;
		while (ind < i + 8) //40
			tmp[j++] = t[ind++];
		tmp[j] = '\0';
		tmpnum = (long)strtol(tmp, NULL, 16);
		sprintf(tmp, "Duration: %ld\n", tmpnum);
		strcat(info, tmp);
	}
	j = 0;
	while (j < 8)
		tmp[j++] = t[ind++];
	tmp[j] = '\0';
	tmpnum = (long)strtol(tmp, NULL,16);
	sprintf(tmp, "Rate: %ld\n", tmpnum);
	strcat(info, tmp);

	j = 0;
	while (j < 4)
		tmp[j++] = t[ind++];
	tmp[j] = '\0';
	tmpnum = (long)strtol(tmp, NULL,16);
	sprintf(tmp, "Volume: %ld\n", tmpnum);
	strcat(info, tmp);
	strcpy(res[strind].description, info);
	res[strind].layer = 1;
	strind++;
	//printf("%s\n", info);
	//printf("ind: %d\n", ind);
	holder = ind;
	return ind;
}

int trak(char *t, int ind)
{
	//layer 1
	char info[100];
	strcpy(res[strind].name, " trak");
	res[strind].layer = 1;
	strind++;
	// printf("ind: %d\n", ind);
	holder = ind;
	return ind + 8;
}

int tkhd(char *t, int ind)
{
	//layer 2
	if (strcmp(res[strind-1].name, " trak") != 0) {
		strcpy(res[strind].name, " trak");
		res[strind].layer = 1;
		strind++;
	}
	char info[1000], tmp[1000];
	int i = 0, j = 0;
	long tmpnum;
	strcpy(res[strind].name, " tkhd");
	strcpy(info, "Box version: ");
	if (t[ind+4] == 0) {
		strcat(info, "1\n");
		ind = ind + 8;
		i = ind;
		while (ind < i + 16)
			tmp[j++] = t[ind++];
		tmp[j] = '\0';
		tmpnum = (long)strtol(tmp, NULL, 16);
		sprintf(tmp, "Creation Time: %ld\n", tmpnum);
		strcat(info, tmp);
		i = i + 16;
		j = 0;
		while (ind < i + 16)
			tmp[j++] = t[ind++];
		tmp[j] = '\0';
		tmpnum = (long)strtol(tmp, NULL, 16);
		sprintf(tmp, "Modification Time: %ld\n", tmpnum);
		strcat(info, tmp);
		i = i + 8;
		j = 0;
		while (ind < i + 8)
			tmp[j++] = t[ind++];
		tmp[j] = '\0';
		tmpnum = (long)strtol(tmp, NULL, 16);
		sprintf(tmp, "Track ID: %ld\n", tmpnum);
		strcat(info, tmp);

		i = i + 8;
		j = 0;
		while (ind < i + 16)
			tmp[j++] = t[ind++];
		tmp[j] = '\0';
		tmpnum = (long)strtol(tmp, NULL, 16);
		sprintf(tmp, "Duration: %ld\n", tmpnum);
		strcat(info, tmp);
	} else {
		strcat(info, "0\n");
		ind = ind + 8;
		i = ind;
		while (ind < i + 8)
			tmp[j++] = t[ind++];
		tmp[j] = '\0';
		tmpnum = (long)strtol(tmp, NULL, 16);
		sprintf(tmp, "Creation Time: %ld\n", tmpnum);
		strcat(info, tmp);
		i = i + 8;
		j = 0;
		while (ind < i + 8)
			tmp[j++] = t[ind++];
		tmp[j] = '\0';
		tmpnum = (long)strtol(tmp, NULL, 16);
		sprintf(tmp, "Modification Time: %ld\n", tmpnum);
		strcat(info, tmp);
		i = i + 8;
		j = 0;
		while (ind < i + 8)
			tmp[j++] = t[ind++];
		tmp[j] = '\0';
		tmpnum = (long)strtol(tmp, NULL, 16);
		sprintf(tmp, "Track ID: %ld\n", tmpnum);
		strcat(info, tmp);
		i = i + 16;
		ind = ind + 8;
		j = 0;
		while (ind < i + 8)
			tmp[j++] = t[ind++];
		tmp[j] = '\0';
		tmpnum = (long)strtol(tmp, NULL, 16);
		sprintf(tmp, "Duration: %ld\n", tmpnum);
		strcat(info, tmp);
	}
	j = 0;
	ind = ind + 104;
	while (j < 4)
		tmp[j++] = t[ind++];
	tmp[j] = '\0';
	tmpnum = (long)strtol(tmp, NULL,16);
	sprintf(tmp, "Width: %ld\n", tmpnum);
	strcat(info, tmp);

	ind = ind + 4;
	j = 0;
	while (j < 4)
		tmp[j++] = t[ind++];
	tmp[j] = '\0';
	tmpnum = (long)strtol(tmp, NULL,16);
	sprintf(tmp, "Height: %ld\n", tmpnum);
	strcat(info, tmp);
	//printf("%s\n", info);
	strcpy(res[strind].description, info);
	res[strind].layer = 2;
	strind++;
	//printf("ind: %d\n", ind);
	holder = ind;
	if (ind % 8 != 0)
		return ind + 4;
	return ind;
}

int tref(char *t, int ind)
{
	//layer 2
	char info[1000], tmp[1000];
	int tmpnum, i = 0, j = 0;
	strcpy(info, " tref");
	strcpy(res[strind].name, info);
	res[strind].layer = 2;
	strcpy(info, "Track IDs: ");
	ind = ind + 8;
	i = ind;
	while (ind < i + 8)
		tmp[i++] = t[ind++];
	tmp[ind] = '\n';
	tmp[ind+1] = '\0';
	strcat(info, tmp);
	strcpy(res[strind].description, info);
	strind++;
	// printf("ind: %d\n", ind);
	holder = ind;
	return ind + 8;
}

int edts(char *t, int ind)
{
	//layer 2
	char info[1000];
	strcpy(info, " edts");
	strcpy(res[strind].name, info);
	res[strind].layer = 2;
	strind++;
	//printf("%s\n", info);
	// printf("ind: %d\n", ind);
	holder = ind;
	return ind + 8;
}

int elst(char *t, int ind)
{
	//layer 3
	char info[1000], tmp[1000];
	if (strcmp(res[strind-1].name, " edts") != 0) {
		holder += 2;
		return holder;
	}
	strcpy(res[strind].name, " elst");
	res[strind].layer = 3;
	int i = 0, j = 0, hold = 0, l = 0;
	long tmpnum;
	i = ind;
	while (ind < i + 8)
		tmp[j++] = t[ind++];
	tmp[j] = '\0';
	tmpnum = (long)strtol(tmp, NULL, 16);
	l = tmpnum;
	sprintf(tmp, "Version: %ld\n", tmpnum);
	strcpy(info, tmp);
	j = 0;
	i = i + 8;
	if (l == 0)
		hold = 16;
	else
		hold = 8;

	while (ind < i + 8)
		tmp[j++] = t[ind++];
	tmp[j] = '\0';
	tmpnum = (long)strtol(tmp, NULL, 16);
	l = tmpnum;
	sprintf(tmp, "Entries Count: %ld\n", tmpnum);
	strcat(info, tmp);
	j = 0;
	ind = ((l * hold) + ind);
	j = 0;
	while (j < 4)
		tmp[j++] = t[ind++];
	tmp[j] = '\0';
	tmpnum = (long)strtol(tmp, NULL, 16);
	sprintf(tmp, "Media Rate: %ld\n", tmpnum);
	strcat(info, tmp);
	//printf("%s\n", info);
	strcpy(res[strind].description, info);
	strind++;
	//printf("ind: %d\n", ind);
	// if (ind % 8 != 0) {
	// 	return (ind - (8 - ind % 8));
	// }
	holder = ind;
	return ind + 4;
}

int mdia(char *t, int ind)
{
	//layer 2
	if (strcmp(res[strind-1].name, " tkhd") != 0) {
		holder += 2;
		return holder;
	}
	char info[100];
	strcpy(info, " mdia");
	strcpy(res[strind].name, info);
	res[strind].layer = 2;
	strind++;
	//printf("%s\n", info);
	// printf("ind: %d\n", ind);
	holder = ind;
	return ind + 8;
}

int mdhd(char *t, int ind)
{
	//layer 3
	char info[100], tmp[100];
	int i = 0, j = 0;
	long tmpnum;
	if (strcmp(res[strind-1].name, " mdia") != 0) {
		holder += 2;
		return holder;
	}
	strcpy(res[strind].name, " mdhd");
	strcpy(info, "Box Version: ");
	if (t[ind+4] == 1) {
		strcat(info, "1\n");
		ind = ind + 8;
		i = ind;
		while (ind < i + 16)
			tmp[j++] = t[ind++];
		tmp[j] = '\0';
		tmpnum = (long)strtol(tmp, NULL, 16);
		sprintf(tmp, "Creation Time: %ld\n", tmpnum);
		strcat(info, tmp);
		i = i + 16;
		j = 0;
		while (ind < i + 16)
			tmp[j++] = t[i++];
		tmp[j] = '\0';
		tmpnum = (long)strtol(tmp, NULL, 16);
		sprintf(tmp, "Modification Time: %ld\n", tmpnum);
		strcat(info, tmp);
		i = i + 8;
		j = 0;
		while (ind < i + 8)
			tmp[j++] = t[ind++];
		tmp[j] = '\0';
		tmpnum = (long)strtol(tmp, NULL, 16);
		sprintf(tmp, "Timescale: %ld\n", tmpnum);
		strcat(info, tmp);
		i = i + 16;
		j = 0;
		while (ind < i + 16)
			tmp[j++] = t[ind++];
		tmp[j] = '\0';
		tmpnum = (int)strtol(tmp, NULL, 16);
		sprintf(tmp, "Duration: %ld\n", tmpnum);
		strcat(info, tmp);
	} else {
		strcat(info, "0\n");
		ind = ind + 8;
		i = ind;
		while (ind < i + 8)
			tmp[j++] = t[ind++];
		tmp[j] = '\0';
		tmpnum = (long)strtol(tmp, NULL, 16);
		sprintf(tmp, "Creation Time: %ld\n", tmpnum);
		strcat(info, tmp);
		i = i + 8;
		j = 0;
		while (ind < i + 8)
			tmp[j++] = t[ind++];
		tmp[j] = '\0';
		tmpnum = (long)strtol(tmp, NULL, 16);
		sprintf(tmp, "Modification Time: %ld\n", tmpnum);
		strcat(info, tmp);
		i = i + 8;
		j = 0;
		while (ind < i + 8)
			tmp[j++] = t[ind++];
		tmp[j] = '\0';
		tmpnum = (long)strtol(tmp, NULL, 16);
		sprintf(tmp, "Timescale: %ld\n", tmpnum);
		strcat(info, tmp);
		i = i + 8;
		j = 0;
		while (ind < i + 8)
			tmp[j++] = t[ind++];
		tmp[j] = '\0';
		tmpnum = (long)strtol(tmp, NULL, 16);
		sprintf(tmp, "Duration: %ld\n", tmpnum);
		strcat(info, tmp);
	}
	//printf("%s\n", info);
	strcpy(res[strind].description, info);
	res[strind].layer = 3;
	strind++;
	holder = ind;
	return ind;
}

int hdlr(char *t, int ind)
{
	//layer 3
	if (strcmp(res[strind-1].name, " mdhd") != 0) {
		holder += 2;
		return holder;
	}
	char info[1000], tmp[1000], tmp2[2], tmp3[1000], tmpr[1000];
	int i = 0, j = 0, k=0, tmpchar;
	long tmpnum;
	strcpy(res[strind].name, " hdlr");
	res[strind].layer = 3;
	ind = ind + 16;
	i = ind;
	while (ind < i + 8)
		tmp[j++] = t[ind++];
	tmp[j] = '\0';
	tmpnum = (long)strtol(tmp, NULL, 16);
	sprintf(tmp, "Handler Type: %ld\nName: ", tmpnum);
	strcpy(info, tmp);
	tmp[0] = '\0';
	ind= ind + 24;
	j = 0;
	while (t[ind] != '0') {
		tmp[j++] = t[ind++];
		tmp[j++] = t[ind++];
		tmpr[k++] = tmp[j-2];
		tmpr[k] = tmp[j-1];
		tmpchar = (int)strtol(tmpr, NULL, 16);
		k = 0;
	}
	k = 0;
	for (j = 0; j < strlen(tmp); ) {
		tmp2[k++] = tmp[j++];
		tmp2[k++] = tmp[j++];
		tmpchar = (int)strtol(tmp2, NULL, 16);
		sprintf(tmp3, "%c", tmpchar);
		strcat(info, tmp3);
		k=0;
	}
	//printf("%s\n", info);
	strcat(info, "\n");
	strcpy(res[strind].description, info);
	strind++;
	// printf("ind: %d\n", ind);
	holder = ind;
	return ind + 8;
}

int minf(char *t, int ind)
{
	//layer 3
	if (strcmp(res[strind-1].name, " hdlr") != 0) {
		holder += 2;
		return holder;
	}
	char info[100];
	strcpy(res[strind].name, " minf");
	res[strind].layer = 3;
	strind++;
	holder = ind;
	return ind + 8;
}

int vmhd(char *t, int ind)
{
	//layer 4
	char info[100], tmp[100];
	strcpy(res[strind].name, " vmhd");
	res[strind].layer = 4;
	int tmpnum, i = 0, j = 0;
	ind = ind + 8;
	i = ind;
	while (ind < i + 16)
		tmp[j++] = t[ind++];
	tmp[j] = '\0';
	tmpnum = (int)strtol(tmp, NULL, 16);
	sprintf(tmp, "Graphics Mode: %d\n", tmpnum);
	strcat(info, tmp);
	//printf("%s\n", info);
	strcpy(res[strind].description, info);
	strind++;
	holder = ind;
	return ind + 8;
}

int smhd(char *t, int ind)
{
	//layer 4
	char info[100], tmp[100];
	strcpy(res[strind].name, " smhd");
	res[strind].layer = 4;
	int tmpnum, i = 0, j = 0;
	ind = ind + 8;
	while (ind < i + 4)
		tmp[j++] = t[ind++];
	tmp[j] = '\0';
	tmpnum = (int)strtol(tmp, NULL, 16);
	sprintf(tmp, "Balance: %d\n", tmpnum);
	strcat(info, tmp);
	//printf("%s\n", info);
	strcpy(res[strind].description, info);
	holder = ind;
	strind++;
	return ind + 8;
}

int hmhd(char *t, int ind)
{
	//layer 4
	char info[100], tmp[100];
	strcpy(res[strind].name, " hmhd");
	res[strind].layer = 4;
	int tmpnum, i = 0, j = 0;
	ind = ind + 8;
	while (ind < i + 4)
		tmp[j++] = t[ind++];
	tmp[j] = '\0';
	tmpnum = (int)strtol(tmp, NULL, 16);
	sprintf(tmp, "Max PDU Size: %d\n", tmpnum);
	strcat(info, tmp);
	i = i + 4;
	j = 0;
	while (ind < i + 4)
		tmp[j++] = t[ind++];
	tmp[j] = '\0';
	tmpnum = (int)strtol(tmp, NULL, 16);
	sprintf(tmp, "Average PDU Size: %d\n", tmpnum);
	strcat(info, tmp);
	i = i + 8;
	j = 0;
	while (ind < i + 8)
		tmp[j++] = t[ind++];
	tmp[j] = '\0';
	tmpnum = (int)strtol(tmp, NULL, 16);
	sprintf(tmp, "Max Bitrate: %d\n", tmpnum);
	strcat(info, tmp);
	i = i + 8;
	j = 0;
	while (ind < i + 8)
		tmp[j++] = t[ind++];
	tmp[j] = '\0';
	tmpnum = (int)strtol(tmp, NULL, 16);
	sprintf(tmp, "Average Bitrate: %d\n", tmpnum);
	strcat(info, tmp);
	//printf("%s\n", info);
	strcpy(res[strind].description, info);
	holder = ind;
	strind++;
	return ind + 8;
}

int nmhd(char *t, int ind)
{
	//layer 4
	char info[100];
	strcpy(res[strind].name, " nmhd");
	res[strind].layer = 4;
	strind++;
	//printf("%s\n", info);
	holder = ind;
	return ind + 8;
}

int dinf(char *t, int ind)
{
	//layer 4
	if (strcmp(res[strind-1].name, " dinf") != 0) {
		holder += 2;
		return holder;
	}
	char info[100];
	strcpy(res[strind].name, " dinf");
	res[strind].layer = 4;
	strind++;
	//printf("%s\n", info);
	holder = ind;
	return ind + 8;
}

int dref(char *t, int ind)
{
	//layer 5
	if (strcmp(res[strind-1].name , " dinf") != 0) {
		strcpy(res[strind].name, " dinf");
		res[strind].layer = 2;
		strind++;
	}
	char info[100];
	strcpy(res[strind].name, " dref");
	res[strind].layer = 5;
	strind++;
	holder = ind;
	return ind + 8;
}

int stbl(char *t, int ind)
{
	//layer 4
	char info[100];
	strcpy(res[strind].name, " stbl");
	res[strind].layer = 4;
	strind++;
	//printf("%s\n", info);
	holder = ind;
	return ind + 8;
}

int stsd(char *t, int ind)
{
	//layer 5
	char info[100], tmp[100];
	if (strcmp(res[strind-1].name , " stbl") != 0) {
		strcpy(res[strind].name, " stbl");
		res[strind].layer = 4;
		strind++;
	}
	strcpy(res[strind].name, " stsd");
	res[strind].layer = 5;
	int tmpnum, i = 0, j = 0;
	ind = ind + 8;
	i = ind;
	while (ind < i + 8)
		tmp[j++] = t[ind++];
	tmp[j] = '\0';
	tmpnum = (int)strtol(tmp, NULL, 16);
	sprintf(tmp, "Entry Count: %d\n", tmpnum);
	strcat(res[strind].description, tmp);
	strind++;
	holder = ind;
	//printf("%s\n", info);
	return ind + 8;
}

int stts(char *t, int ind)
{
	//layer 5
	char info[100], tmp[100];
	strcpy(res[strind].name, " stts");
	res[strind].layer = 5;
	int tmpnum, i = 0, j = 0;
	ind = ind + 8;
	i = ind;
	while (ind < i + 8)
		tmp[j++] = t[ind++];
	tmp[j] = '\0';
	tmpnum = (int)strtol(tmp, NULL, 16);
	sprintf(tmp, "Entry Count: %d\n", tmpnum);
	strcat(info, tmp);
	//printf("%s\n", info);
	strcpy(res[strind].description, info);
	strind++;
	holder = ind;
	return ind + 8;
}

int stsc(char *t, int ind)
{
	//layer 5
	char info[100], tmp[100];
	strcpy(res[strind].name, " stsc");
	res[strind].layer = 5;
	int tmpnum, i = 0, j = 0;
	ind = ind + 8;
	i = ind;
	while (ind < i + 8)
		tmp[j++] = t[ind++];
	tmp[j] = '\0';
	tmpnum = (int)strtol(tmp, NULL, 16);
	sprintf(tmp, "Entry Count: %d\n", tmpnum);
	strcat(info, tmp);
	//printf("%s\n", info);
	strcpy(res[strind].description, info);
	strind++;
	holder = ind;
	return ind + 8;

}

int stco(char *t, int ind)
{
	//layer 5
	char info[100], tmp[100];
	strcpy(res[strind].name, " stco");
	res[strind].layer = 5;
	int tmpnum, i = 0, j = 0;
	ind = ind + 8;
	i = ind;
	while (ind < i + 8)
		tmp[j++] = t[ind++];
	tmp[j] = '\0';
	tmpnum = (int)strtol(tmp, NULL, 16);
	sprintf(tmp, "Entry Count: %d\n", tmpnum);
	strcat(info, tmp);
	//printf("%s\n", info);
	strcpy(res[strind].description, info);
	strind++;
	holder = ind;
	return ind + 8;
}

int mvex(char *t, int ind)
{
	//layer 1
	char info[100];
	//printf("%s\n", info);
	strcpy(res[strind].name, " mvex");
	res[strind].layer = 1;
	strind++;
	holder = ind;
	return ind + 8;
}

int trex(char *t, int ind)
{
	//layer 2
	char info[100], tmp[100];
	strcpy(res[strind].name, " trex");
	res[strind].layer = 2;
	int tmpnum, i = 0, j = 0;
	ind = ind + 8;
	i = ind;
	while (ind < i + 8)
		tmp[j++] = t[ind++];
	tmp[j] = '\0';
	tmpnum = (int)strtol(tmp, NULL, 16);
	sprintf(tmp, "Track ID: %d\n", tmpnum);
	strcat(info, tmp);
	i = i + 8;
	j = 0;
	while (ind < i + 8)
		tmp[j++] = t[ind++];
	tmp[j] = '\0';
	tmpnum = (int)strtol(tmp, NULL, 16);
	sprintf(tmp, "Sample Description Index: %d\n", tmpnum);
	strcat(info, tmp);
	i = i + 8;
	j = 0;
	while (ind < i + 8)
		tmp[j++] = t[i++];
	tmp[j] = '\0';
	tmpnum = (int)strtol(tmp, NULL, 16);
	sprintf(tmp, "Sample Duration: %d\n", tmpnum);
	strcat(info, tmp);
	i = i + 8;
	j = 0;
	while (ind < i + 8)
		tmp[j++] = t[ind++];
	tmp[j] = '\0';
	tmpnum = (int)strtol(tmp, NULL, 16);
	sprintf(tmp, "Sample Size: %d\n", tmpnum);
	strcat(info, tmp);
	i = i + 8;
	j = 0;
	while (ind < i + 8)
		tmp[j++] = t[ind++];
	tmp[j] = '\0';
	tmpnum = (int)strtol(tmp, NULL, 16);
	sprintf(tmp, "Sample Flags: %d\n", tmpnum);
	strcat(info, tmp);
	//printf("%s\n", info);
	strcpy(res[strind].description, info);
	strind++;
	holder = ind;
	return ind + 8;
}

int moof(char *t, int ind)
{
	//layer 0
	char info[100];
	strcpy(res[strind].name, " moof");
	res[strind].layer = 0;
	strind++;
	//printf("%s\n", info);
	holder = ind;
	return ind + 8;
}

int mfhd(char *t, int ind)
{
	//layer 1
	char info[100], tmp[100];
	strcpy(res[strind].name, " mfhd");
	res[strind].layer = 1;
	int tmpnum, i = 0, j = 0;
	ind = ind + 8;
	i = ind;
	while (ind < i + 8)
		tmp[j++] = t[ind++];
	tmp[j] = '\0';
	tmpnum = (int)strtol(tmp, NULL, 16);
	sprintf(tmp, "Sequence Number: %d\n", tmpnum);
	strcat(info, tmp);
	//printf("%s\n", info);
	strcpy(res[strind].description, info);
	strind++;
	holder = ind;
	return ind + 8;
}

int traf(char *t, int ind)
{
	//layer 1
	char info[100];
	strcpy(res[strind].name, " traf");
	res[strind].layer = 1;
	strind++;
	//printf("%s\n", info);
	holder = ind;
	return ind + 8;
}

int tfhd(char *t, int ind)
{
	//layer 2
	char info[100], tmp[100];
	strcpy(res[strind].name, " tfhd");
	res[strind].layer = 2;
	int tmpnum, i = 0, j = 0;
	ind = ind + 8;
	i = ind;
	while (ind < i + 8)
		tmp[j++] = t[ind++];
	tmp[j] = '\0';
	tmpnum = (int)strtol(tmp, NULL, 16);
	sprintf(tmp, "Track ID: %d\n", tmpnum);
	strcat(info, tmp);
	//printf("%s\n", info);
	strcpy(res[strind].description, info);
	strind++;
	holder = ind;
	return ind + 8;
}

int mfra(char *t, int ind)
{
	//layer 0
	char info[100];
	strcpy(res[strind].name, " mfra");
	res[strind].layer = 0;
	strind++;
	//printf("%s\n", info);
	holder = ind;
	return ind + 8;
}

int mfro(char *t, int ind)
{
	//layer 1
	char info[100], tmp[100];
	strcpy(res[strind].name, " mfro");
	res[strind].layer = 1;
	int tmpnum, i = 0, j = 0;
	ind = ind + 8;
	i = ind;
	while (ind < i + 8)
		tmp[j++] = t[ind++];
	tmp[j] = '\0';
	tmpnum = (int)strtol(tmp, NULL, 16);
	sprintf(tmp, "Size: %d\n", tmpnum);
	strcat(info, tmp);
	//printf("%s\n", info);
	strcpy(res[strind].description, info);
	strind++;
	holder = ind;
	return ind + 8;
}

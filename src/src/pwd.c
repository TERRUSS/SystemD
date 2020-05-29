#include <stdio.h>
#include <stdlib.h>
#include "../fs/fs.h"

int main(int argc, char const *argv[]) {
	unsigned int cur_dir = get_pwd_id();


	struct inode cur = get_inode_by_id(cur_dir);
	struct inode inodelist[100];
	int count = 0, *subid;
	struct bloc b;

	inodelist[0] = cur;

	while( inodelist[count].id != ROOT_ID ){
		count++;
		b = get_bloc_by_id(inodelist[count-1].bloc_ids[0]);
		strsplt(b.content, &subid, ',');
		
		inodelist[count] = get_inode_by_id(subid[1]);
		printf("%d : %d\n", count, inodelist[count].id);	
	}

	for (int i = count; i > -1; i--){
		printf("%s", get_dirname_by_id(inodelist[i].id));
	}
	
	
	printf("\n");

	return 0;
}

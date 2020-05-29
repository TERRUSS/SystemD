#include <stdio.h>
#include <stdlib.h>
#include "../fs/fs.h"

char * handleArgs(int argc, char const *argv[]) {
	if(argc == 2)
		return argv[1];
	else {
		exit(-1);
	}
}

char * rec(struct inode * inode, char * name){
	char * tmp = 0;
	if ( inode->type == DIRECTORY ){

		int *subid;
		struct bloc b;
		struct inode ** sub;
		b = get_bloc_by_id(inode->bloc_ids[0]);
		int sub_len = strsplt(b.content, &subid, ',');
		sub = malloc(sizeof(struct inode *) * sub_len);

		for (int i = 0; i < sub_len; i++) {
			struct inode tmpinode = get_inode_by_id(subid[i]);
			sub[i] = (struct inode *) malloc(sizeof(struct inode *));
			memcpy(sub[i], &tmpinode, sizeof(struct inode));

			char * tmp = rec(sub[i], name);

			if (tmp){
				char * ret = malloc(sizeof(char) * (strlen(get_filename_for_inode(inode))+ strlen(tmp))+1);
				memcpy(ret, get_filename_for_inode(inode), strlen(get_filename_for_inode(inode)));
				ret[strlen(get_filename_for_inode(inode))] = '/';
				memcpy(ret+strlen(get_filename_for_inode(inode))+1, tmp, strlen(tmp));
				free(tmp);

				if (inode->id == ROOT_ID){
					printf("%s\n", ret);
					exit(0);
				}
				return ret;
			}
			else{
				return 0;
			}
			
		}
	}
	else{
		if (strcmp( get_filename_for_inode(inode), name) == 0){
			return get_filename_for_inode(inode);
		}
	}
	
	return 0;
}

int main(int argc, char const *argv[]) {

	char * name = handleArgs(argc, argv);

	struct inode root = get_inode_by_id(ROOT_ID);

	rec(&root, name);

	return 0;
}

/* Ethan
* myls
*/

#include <unistd.h>
#include <pwd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <time.h>
#include <stdio.h>
#include <dirent.h>
#include <errno.h>
#include <stdlib.h>
#include <grp.h>
#include <locale.h>
#include <langinfo.h>
#include <stdint.h>
#include <string.h>

void myls_file(char *file, int arg_l){
    struct stat attrib;
    struct passwd *pwd;
    struct group *grp;

    if(stat(file, &attrib) == -1){
        perror("Trouble accessing the file.");
				exit(EXIT_FAILURE);
    }

    if(arg_l){
			/* prints permission info:
			It checks the attributes of the file and uses a conditional statement
			to determine whethere we want to print the letter corresponding with the
			permission statement or if we want the dash*/
			printf( (S_ISDIR(attrib.st_mode)) ? "d" : "-");
			printf( (attrib.st_mode & S_IRUSR) ? "r" : "-");
			printf( (attrib.st_mode & S_IWUSR) ? "w" : "-");
			printf( (attrib.st_mode & S_IXUSR) ? "x" : "-");
			printf( (attrib.st_mode & S_IRGRP) ? "r" : "-");
			printf( (attrib.st_mode & S_IWGRP) ? "w" : "-");
			printf( (attrib.st_mode & S_IXGRP) ? "x" : "-");
			printf( (attrib.st_mode & S_IROTH) ? "r" : "-");
			printf( (attrib.st_mode & S_IWOTH) ? "w" : "-");
			printf( (attrib.st_mode & S_IXOTH) ? "x" : "-");

			/*prints the number of links*/
			printf("%4d ", (int)attrib.st_nlink);

			/*prints the owner's name*/
			if((pwd = getpwuid(attrib.st_uid)) != NULL){
				printf(" %-8.8s ", pwd -> pw_name);
			} else {
				printf(" %-8d ", attrib.st_gid);
			}

			/*prints the group name*/
			if ((grp = getgrgid(attrib.st_gid)) != NULL){
				printf(" %-8.8s ", grp -> gr_name);
			} else {
				printf(" %-8d ", attrib.st_gid);
			}

			/*prints the size of the block*/
			printf(" %9jd ", (intmax_t)attrib.st_size);
			char time[50];
			strftime(time, 50, "%b %d %H:%M  ", localtime(&attrib.st_mtime));
			printf ("%s", time);
		}
    printf("%s\n", file);
}


void myls(const char *dir, int arg_a,int arg_l, int ext){
    struct dirent *d;
    struct passwd *pwd;
    struct group *grp;
    struct stat attrib;
    DIR *dh = opendir(dir);

		/*reads in the directory*/
    if (!dh){
        if (errno = ENOENT){
            perror("Trouble reading the Directory");
					}
	    exit(EXIT_FAILURE);
    }

		/*While the directory we're looking at has more files, the loop iterates*/
    while ((d = readdir(dh)) != NULL){
        if (!arg_a && d->d_name[0] == '.')
            continue;
        if(arg_l == 1){
					if(ext == 1){
						/*new directory passed in*/
						if(stat(dir, &attrib) == -1){
							perror("Trouble accessing the directory.");
							exit(EXIT_FAILURE);
						}
					} else {
						/*read file in current directory*/
						stat(d -> d_name, &attrib);
					}
					/*print permission info*/
          printf( (S_ISDIR(attrib.st_mode)) ? "d" : "-");
          printf( (attrib.st_mode & S_IRUSR) ? "r" : "-");
          printf( (attrib.st_mode & S_IWUSR) ? "w" : "-");
          printf( (attrib.st_mode & S_IXUSR) ? "x" : "-");
          printf( (attrib.st_mode & S_IRGRP) ? "r" : "-");
          printf( (attrib.st_mode & S_IWGRP) ? "w" : "-");
          printf( (attrib.st_mode & S_IXGRP) ? "x" : "-");
          printf( (attrib.st_mode & S_IROTH) ? "r" : "-");
          printf( (attrib.st_mode & S_IWOTH) ? "w" : "-");
          printf( (attrib.st_mode & S_IXOTH) ? "x" : "-");

          //prints the number of links
          printf("%4d ", (int)attrib.st_nlink);
          //owner's name
          if((pwd = getpwuid(attrib.st_uid)) != NULL){
            printf(" %-8.8s ", pwd -> pw_name);
          } else {
            printf(" %-8d ", attrib.st_gid);
          }
          // group name
          if ((grp = getgrgid(attrib.st_gid)) != NULL){
            printf(" %-8.8s ", grp -> gr_name);
          } else {
            printf(" %-8d ", attrib.st_gid);
          }

          //size of the block
          printf(" %9jd ", (intmax_t)attrib.st_size);

          char time[50];
          strftime(time, 50, "%b %d %H:%M  ", localtime(&attrib.st_mtime));
          printf ("%s", time);
      	}
      	printf("%s  ", d->d_name);

				if((arg_a == 1 && arg_l == 0) || (arg_a == 0 && arg_l == 0)){
            continue;
        } else {
            printf("\n");
        }
    }
    printf("\n");
}

int main(int argc, char *argv[]) {
    int opt;
    int arg_a = 0, arg_l = 0;

    while((opt = getopt(argc, argv, ":al")) != -1){
			switch(opt) {
            case 'a':
                arg_a = 1;
                break;
            case 'l':
                arg_l = 1;
                break;
            case '?':
                printf("unknown option: %c\n", optopt);
		exit(EXIT_FAILURE);
                break;
        }
    }


    if(argv[optind] == NULL){
			/*basic ls with no argument*/
			myls(".", arg_a, arg_l, 0);
		} else {
			while(optind <= argc-1){
				int is_dir=0, is_file=0;
				/*check is file or directory*/
				char *name = argv[optind];
				struct stat st_buf;
				stat(name, &st_buf);
				//Checks to see what we are reading
				if(S_ISDIR(st_buf.st_mode)){
					is_dir = 1;
				} else if (S_ISREG(st_buf.st_mode)){
					is_file = 1;
				} else {
					perror("Trouble accessing the directory.");
					exit(EXIT_FAILURE);
				}

				/* operate on directory */
				if(is_dir){
					/*retrive path to current working directory*/
					char cwd[PATH_MAX];
					if (getcwd(cwd, sizeof(cwd)) == NULL) {
						perror("getcwd() error");
						exit(EXIT_FAILURE);
					}
					printf("%s:\n", name);
					char *path1 = strcat(strcat(cwd, "/"), name);
					myls(path1, arg_a, arg_l, is_dir);
				}

				/* operate on file */
				if(is_file){
					printf("%s:\n", name);
					myls_file(name, arg_l);
	    }
	    /*read next item*/
	    optind+=1;
		}
	}
  return 0;
}

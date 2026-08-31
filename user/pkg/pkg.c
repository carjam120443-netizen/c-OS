/* c-OS package manager prototype. */
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include "pkg.h"

static const char *db_dir(void) { const char *v=getenv("COS_PKG_DB"); return v&&*v?v:PKG_DB_DIR; }
static const char *repo_file(void) { const char *v=getenv("COS_PKG_REPO"); return v&&*v?v:PKG_REPO_FILE; }
static int ensure_dir(const char *p) { return mkdir(p,0755)==0 || errno==EEXIST ? 0 : -1; }
static int package_path(char *out,size_t n,const char *name) { if(!name||!*name||strchr(name,'/')||strstr(name,"..")) return -1; return snprintf(out,n,"%s/%s.pkg",db_dir(),name)<(int)n?0:-1; }
static int installed(const char *name) { char p[PKG_MAX_PATH]; struct stat s; return package_path(p,sizeof(p),name)==0&&stat(p,&s)==0; }

static void usage(void) {
 puts("cOS-pkg - c-OS package manager");
 puts("Usage:");
 puts("  pkg install <package>...   Install packages");
 puts("  pkg remove <package>...    Remove packages");
 puts("  pkg info <package>         Show package information");
 puts("  pkg list                   List installed packages");
 puts("  pkg search <term>          Search repository metadata");
 puts("  pkg update                 Refresh repository metadata");
 puts("  pkg upgrade                Upgrade installed packages");
 puts("  pkg check                  Check package database");
 puts("  pkg help                   Show this help");
}

static int install_pkg(const char *name) {
 char p[PKG_MAX_PATH]; FILE *f;
 if(package_path(p,sizeof(p),name)){fprintf(stderr,"pkg: invalid package name '%s'\n",name);return 2;}
 if(installed(name)){printf("pkg: %s is already installed\n",name);return 0;}
 if(ensure_dir(db_dir())){fprintf(stderr,"pkg: cannot create %s: %s\n",db_dir(),strerror(errno));return 1;}
 f=fopen(p,"w"); if(!f){fprintf(stderr,"pkg: cannot install %s: %s\n",name,strerror(errno));return 1;}
 fprintf(f,"name=%s\nversion=0.1\ndescription=c-OS package\nstatus=installed\n",name); fclose(f);
 printf("pkg: installed %s\n",name); return 0;
}
static int remove_pkg(const char *name) {
 char p[PKG_MAX_PATH];
 if(package_path(p,sizeof(p),name)||!installed(name)){fprintf(stderr,"pkg: %s is not installed\n",name);return 1;}
 if(remove(p)){fprintf(stderr,"pkg: cannot remove %s: %s\n",name,strerror(errno));return 1;}
 printf("pkg: removed %s\n",name); return 0;
}
static int info_pkg(const char *name) {
 char p[PKG_MAX_PATH],line[512]; FILE *f;
 if(package_path(p,sizeof(p),name)||!installed(name)){fprintf(stderr,"pkg: %s is not installed\n",name);return 1;}
 f=fopen(p,"r"); if(!f)return 1; while(fgets(line,sizeof(line),f))fputs(line,stdout); fclose(f); return 0;
}
static int list_pkg(void) {
 char cmd[PKG_MAX_PATH+32]; if(ensure_dir(db_dir()))return 1;
 snprintf(cmd,sizeof(cmd),"ls -1 '%s' 2>/dev/null",db_dir()); return system(cmd)==0?0:1;
}
static int search_pkg(const char *term) {
 FILE *f=fopen(repo_file(),"r"); char line[512];
 if(!f){fprintf(stderr,"pkg: repository metadata unavailable: %s\n",repo_file());return 1;}
 while(fgets(line,sizeof(line),f))if(strstr(line,term))fputs(line,stdout); fclose(f); return 0;
}
static int update_repo(void) {
 FILE *f=fopen(repo_file(),"a"); if(!f){fprintf(stderr,"pkg: cannot open repository metadata: %s\n",strerror(errno));return 1;}
 fclose(f); printf("pkg: repository metadata ready: %s\n",repo_file()); return 0;
}
static int check_db(void) { if(ensure_dir(db_dir())){fprintf(stderr,"pkg: package database unavailable\n");return 1;} printf("pkg: database is accessible: %s\n",db_dir()); return 0; }
static int upgrade(void) { puts("pkg: checking installed packages for upgrades..."); puts("pkg: no remote package index configured yet"); return 0; }

int main(int argc,char **argv) {
 int rc=0;
 if(argc<2||!strcmp(argv[1],"help")){usage();return 0;}
 if(!strcmp(argv[1],"install")){if(argc<3){fprintf(stderr,"pkg: install requires a package name\n");return 2;}for(int i=2;i<argc;i++)if(install_pkg(argv[i]))rc=1;return rc;}
 if(!strcmp(argv[1],"remove")){if(argc<3){fprintf(stderr,"pkg: remove requires a package name\n");return 2;}for(int i=2;i<argc;i++)if(remove_pkg(argv[i]))rc=1;return rc;}
 if(!strcmp(argv[1],"info")){if(argc<3){fprintf(stderr,"pkg: info requires a package name\n");return 2;}return info_pkg(argv[2]);}
 if(!strcmp(argv[1],"list"))return list_pkg();
 if(!strcmp(argv[1],"search")){if(argc<3){fprintf(stderr,"pkg: search requires a term\n");return 2;}return search_pkg(argv[2]);}
 if(!strcmp(argv[1],"update"))return update_repo();
 if(!strcmp(argv[1],"upgrade"))return upgrade();
 if(!strcmp(argv[1],"check"))return check_db();
 fprintf(stderr,"pkg: unknown command '%s'\n",argv[1]); usage(); return 2;
}

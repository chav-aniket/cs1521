// myls.c ... my very own "ls" implementation

#include <sys/types.h>
#include <sys/stat.h>

#include <dirent.h>
#include <err.h>
#include <errno.h>
#include <fcntl.h>
#include <grp.h>
#include <pwd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#ifdef __linux__
# include <bsd/string.h>
#endif
#include <sysexits.h>
#include <unistd.h>
#include <limits.h>

#define MAXDIRNAME 	256
#define MAXFNAME 	256
#define MAXNAME 	24

char *rwxmode (mode_t, char *);
char *username (uid_t, char *);
char *groupname (gid_t, char *);

int main (int argc, char *argv[])
{
	// string buffers for various names
	char uname[MAXNAME+1];
	char gname[MAXNAME+1];
	char mode[MAXNAME+1];
	char path[PATH_MAX];

	// collect the directory name, with "." as default
	char dirname[MAXDIRNAME] = ".";
	if (argc >= 2)
		strlcpy (dirname, argv[1], MAXDIRNAME);

	// check that the name really is a directory
	struct stat info;
	if (stat (dirname, &info) < 0)
		err (EX_OSERR, "%s", dirname);

	if (! S_ISDIR (info.st_mode)) {
		errno = ENOTDIR;
		err (EX_DATAERR, "%s", dirname);
	}

	// open the directory to start reading
	DIR *df;
	df = opendir(dirname);

	// read directory entries
	struct dirent *entry;

	while ((entry = readdir(df)) != NULL) {
		if (entry->d_name[0] == '.')
			continue;

		snprintf(path, sizeof(path), "%s/%s", dirname, entry->d_name);
		lstat(path, &info);

		switch (info.st_mode & S_IFMT) {
			case S_IFDIR: putchar('d'); break;
			case S_IFLNK: putchar('l'); break;
			case S_IFREG: putchar('-'); break;
			default:	  putchar('?'); break;
		}
		printf(
			"%s  %-8.8s %-8.8s %8lld  %s\n",
			rwxmode(info.st_mode, mode),
			username(info.st_uid, uname),
			groupname(info.st_gid, gname),
			(long long) info.st_size,
			entry->d_name
		);
	}

	// finish up
	closedir(df);

	return EXIT_SUCCESS;
}

// convert octal mode to -rwxrwxrwx string
char *rwxmode (mode_t mode, char *str)
{
	// ... TODO ...
	const char chars[] = "rwxrwxrwx";
	for (size_t i = 0; i < 9; i++) {
		str[i] = (mode & (1 << (8-i))) ? chars[i] : '-';
	}
	str[9] = '\0';
	return str;
}

// convert user id to user name
char *username (uid_t uid, char *name)
{
	struct passwd *uinfo = getpwuid (uid);
	if (uinfo != NULL)
		snprintf (name, MAXNAME, "%s", uinfo->pw_name);
	else
		snprintf (name, MAXNAME, "%d?", (int) uid);
	return name;
}

// convert group id to group name
char *groupname (gid_t gid, char *name)
{
	struct group *ginfo = getgrgid (gid);
	if (ginfo != NULL)
		snprintf (name, MAXNAME, "%s", ginfo->gr_name);
	else
		snprintf (name, MAXNAME, "%d?", (int) gid);
	return name;
}

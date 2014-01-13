git filter-branch -f --commit-filter '
        if [ "$GIT_COMMITTER_NAME" = "Sebastian" ];
		then
			GIT_COMMITTER_NAME="sebrockm";
			GIT_AUTHOR_NAME="sebrockm";
			GIT_COMMITTER_EMAIL="sebrockm@uos.de";
			GIT_AUTHOR_EMAIL="sebrockm@uos.de";
			git commit-tree "$@";
		else
			git commit-tree "$@";
		fi' HEAD

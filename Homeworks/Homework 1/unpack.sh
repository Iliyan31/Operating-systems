#!/bin/bash

if [ "$#" -ne 4 ]; then
	echo "There must be four parameters!" >&2
	exit 1
fi

if [ -z "$1" ]; then 
	echo "The first parameter must be a .zip archive" >&2
	exit 2
fi

if [ -z "$2" ]; then
	echo "The second parameter must be an empty directory!" >&2
	exit 3
fi

if [ -z "$3" ]; then
	echo "The third parameter must be an empty directory!" >&2
	exit 4
fi

if [ -z "$4" ]; then 
	echo "The fourth parameter must be an empty file" >&2
	exit 5
fi

if [ -d "$2" ]; then
	echo "The second parameter mustn't be a working directory!" >&2
	exit 6
fi

if [ -d "$3" ]; then
	echo "The third parameter mustn't be a working directory!" >&2
	exit 7
fi

if [ -f "$4" ]; then 
	echo "The fourth parameter mustn't be an existing file!" >&2
	exit 8
fi

if [ ! -f "$1" ]; then
	echo "The first parameter must be an existing zip archive!" >&2
	exit 9
fi

if  ! ( echo "$4" | egrep -q "*.txt" ); then 
	echo "The fourth filename must end with .txt in order to treat it as standart txt file!" >&2
	exit 10
fi

if ! ( echo "$1" | egrep -q "*.zip" ); then 
	echo "The first parameter must end with .zip archive!" >&2
	exit 11
fi

formatZip="$( file "$1" | cut -d ":" -f 2 | cut -c 2- | cut -d " " -f 1 )"

if [ "$formatZip" != "Zip" ]; then 
   	echo "The first parameter must be an archive of type Zip!" >&2
	exit 12
fi

if [ "$1" == "$2" -o "$1" == "$3" -o "$1" == "$4" -o "$2" == "$3" -o "$2" == "$4" -o "$3" == "$4" ]; then
   	echo "The names of the parameters mustn't be equal!" >&2
	exit 13 
fi

if ! mkdir "$2"; then
	echo "There was problem in creating the dir from second parameter!" >&2
	exit 14
fi

if ! mkdir "$3"; then 
	echo "There was problem in creating the dir from the third parameter!" >&2
	exit 15
fi

if ! touch "$4"; then 
	echo "There was problem in creating the file from the fourth parameter!" >&2
	exit 16
fi
 
zipArch="$1"
passedDir="$2"
failedDir="$3"
report="$4"
tempDir="$(mktemp -d)"

if ! unzip -q "$zipArch" -d "$tempDir"; then 
	echo "The file cannot be unzipped!" >&2
	exit 17
fi

find "$tempDir" -mindepth 1 -maxdepth 1 -type d | sort | while read dir; do	 

	flag1=0
	flag2=0
	flag3=0
	flag4=0
	
	if [ ! -d "$dir" ]; then 
		continue
	fi

	nameDir="$( echo "$dir" | cut -d "-" -f 1 | awk -F "/" '{print $NF}' )"
   	
	if [ -z "$nameDir" ]; then 
		continue
	fi
	
	if [ "$(find "$dir" -type f -printf "%p\n" | wc -l)" -ne 1 ]; then
			
		if [ ! -d "$failedDir"/"$nameDir" ]; then
			if ! mkdir "$failedDir"/"$nameDir"; then
				echo "Error in creating dir for student $nameDir in $failedDir"
				continue
			fi
		fi

		if ! find "$dir" -type f -printf "%p\n" | xargs -I {} cp {} "$failedDir"/"$nameDir"; then 
			echo "There was problem in copying the files to $failedDir/$nameDir!" >&2
			continue
		fi
			
	fi	

	find "$dir" -type f -printf "%p\n" | while read archive; do
		archiveName="$( echo "$archive" | awk -F "/" '{print $NF}' )"
		currArchName="$( echo "$archiveName" | cut -d "." -f 1 )"
		Format="$( file "$archive" | cut -d ":" -f 2 | cut -c 2- | awk -F " " '{if($1 == "POSIX" && $2 == "tar") {print $1, $2} else {print $1}}' )"
		Extension="$( echo "$archiveName" | cut -d "." -f 2-)"
		
		if  [ -z "$currArchName" ]; then 
			continue
		fi
		
		if [ "$currArchName" != "$nameDir" -o "$Extension" != "tar.xz" ]; then 
			flag1=1
		fi

		if [ "$Format" != "XZ" ]; then 
			flag2=1
		fi
	
		if ( echo "$Format" | tr [:upper:] [:lower:] |  egrep -q "(xz|posix tar|gzip|rar|zip|bzip2)" ); then
			
			tempFinishedDir="$(mktemp -d)"
		
			if [ "$Format" == "RAR" ]; then 
				if ! unrar -inul x "$archive" "$tempFinishedDir" 2> /dev/null; then
					if [ ! -d "$failedDir"/"$nameDir" ]; then
						if ! mkdir "$failedDir"/"$nameDir"; then
							echo "Error in creating dir for student $nameDir in $failedDir" >&2
							continue
						fi
					fi

					if ! cp "$archive" "$failedDir"/"$nameDir"; then
						echo "There was problem in copying the archive to $failedDir/$nameDir!" >&2
						continue
					fi
				else
					if [ ! -d "$passedDir "/"$nameDir" ]; then
						if ! mkdir "$passedDir"/"$nameDir"; then
							echo "Error in creating dir for student $nameDir in $passedDir!" >&2
							continue
						fi
					fi

					if [ "$(find "$tempFinishedDir" -mindepth 1 -maxdepth 1 -printf "%p\n" | awk -F '/' '{print $NF}' | egrep -v "^._" | wc -l)" -eq 1 ]; then
						if [ "$(find "$tempFinishedDir" -mindepth 1 -maxdepth 1 -type d -printf "%p\n" | awk -F '/' '{print $NF}' | egrep -v "^._" | wc -l)" -eq 1 ]; then
							name="$(find "$tempFinishedDir" -mindepth 1 -maxdepth 1 -type d -printf "%p\n" | awk -F '/' '{print $NF}' | egrep -v "^._")"
								
							if [ "$name" != "$nameDir" ]; then 
								flag4=1
							fi						
						else
							flag3=1
							flag4=1
						fi
					else
						flag3=1
						flag4=1
					fi

					if ! find "$tempFinishedDir" -type f | xargs -I {} mv {} "$passedDir"/"$nameDir"; then 
						echo "There was error in moving the files to $passedDir/$nameDir !" >&2
						continue
					fi
					
				fi
			elif [ "$Format" == "Zip" ]; then 
				if ! unzip -q "$archive" -d "$tempFinishedDir" 2> /dev/null; then 
					if [ ! -d "$failedDir"/"$nameDir" ]; then
						if ! mkdir "$failedDir"/"$nameDir"; then
							echo "Error in creating dir for student $nameDir in $failedDir" >&2
							continue
						fi
					fi

					if ! cp "$archive" "$failedDir"/"$nameDir"; then
						echo "There was problem in copying the archive to $failedDir/$nameDir!" >&2
						continue
					fi
				else
					if [ ! -d "$passedDir "/"$nameDir" ]; then
						if ! mkdir "$passedDir"/"$nameDir"; then
							echo "Error in creating dir for student $nameDir in $passedDir" >&2
							continue
						fi
					fi

					if [ "$(find "$tempFinishedDir" -mindepth 1 -maxdepth 1 -printf "%p\n" | awk -F '/' '{print $NF}' | egrep -v "^._" | wc -l)" -eq 1 ]; then
						if [ "$(find "$tempFinishedDir" -mindepth 1 -maxdepth 1 -type d -printf "%p\n" | awk -F '/' '{print $NF}' | egrep -v "^._" | wc -l)" -eq 1 ]; then
							name="$(find "$tempFinishedDir" -mindepth 1 -maxdepth 1 -type d -printf "%p\n" | awk -F '/' '{print $NF}' | egrep -v "^._")"
	
							if [ "$name" != "$nameDir" ]; then 
								flag4=1
							fi						
						else 
							flag3=1
							flag4=1
						fi
					else
						flag3=1
						flag4=1
					fi

					if ! find "$tempFinishedDir" -type f | xargs -I {} mv {} "$passedDir"/"$nameDir"; then 
						echo "There was error in moving the files to $passedDir/$nameDir !" >&2
						continue
					fi

				fi
			else  
				if ! tar -xf "$archive" -C "$tempFinishedDir" 2> /dev/null; then 
					if [ ! -d "$failedDir"/"$nameDir" ]; then
						if ! mkdir "$failedDir"/"$nameDir"; then
							echo "Error in creating dir for student $nameDir in $failedDir" >&2
							continue
						fi
					fi

					if ! cp "$archive" "$failedDir"/"$nameDir"; then
						echo "There was problem in copying archive to $failedDir/$nameDir!" >&2
						continue
					fi
				else
					if [ ! -d "$passedDir "/"$nameDir" ]; then
						if ! mkdir "$passedDir"/"$nameDir"; then
							echo "Error in creating dir for student $nameDir in $passedDir" >&2
							continue
						fi
					fi
						
					if [ "$(find "$tempFinishedDir" -mindepth 1 -maxdepth 1 -printf "%p\n" | awk -F '/' '{print $NF}' | egrep -v "^._" | wc -l)" -eq 1 ]; then
						if [ "$(find "$tempFinishedDir" -mindepth 1 -maxdepth 1 -type d -printf "%p\n" | awk -F '/' '{print $NF}' | egrep -v "^._" | wc -l)" -eq 1 ]; then
							name="$(find "$tempFinishedDir" -mindepth 1 -maxdepth 1 -type d -printf "%p\n" | awk -F '/' '{print $NF}' | egrep -v "^._")"
	
							if [ "$name" != "$nameDir" ]; then 
								flag4=1
							fi						
						else 
							flag3=1
							flag4=1
						fi
					else
						flag3=1
						flag4=1
					fi	

					if ! find "$tempFinishedDir" -type f | xargs -I {} mv {} "$passedDir"/"$nameDir"; then 
						echo "There was error in moving the files to $passedDir/$nameDir !" >&2
						continue
					fi
				fi
			fi

			if ! echo "$nameDir $flag1 $flag2 $flag3 $flag4" >> "$report"; then
				echo "There was problem in transfering the data row to $report!" >&2
				continue
			fi

			rm -r "$tempFinishedDir"
		else 
			if [ ! -d "$failedDir"/"$nameDir" ]; then
				if ! mkdir "$failedDir"/"$nameDir"; then
					echo "Error in creating dir for student $nameDir in $failedDir" >&2
					continue
				fi
			fi

			if ! cp "$archive" "$failedDir"/"$nameDir"; then
				echo "There was problem in copying the unknown format to $failedDir/$nameDir!" >&2
				continue
			fi
		fi
	done
done

rm -r "$tempDir"

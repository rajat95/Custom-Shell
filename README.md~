# Custom-Shell
Small Project to understand process management

Instructions
-----------------

1. Running a new Command in foreground

	i) Upon running the program you get "command<<"
	ii)Just type the command and hit enter
	
	Examples:
	command>>ls
	command>>wc filename
	command>>sudo apt-get update 
	
	etc....
	
2. Running command in background
	
	i) Append & to the command above
	
	Examples:
	command>>ls&
	command>>sleep 100& (Note: Since sleep works in background shell wont wait for it, so can be used for testing.)
	
3. Viewing list of jobs in Background
	
	i) Just type jobs and press enter.
	ii) Output will be of in format "pid status command" where status tells if process is running or stopped
	
	Example:
	command>>sleep 500 &
	command>>jobs
	
	Output: 4039 running sleep
	
4. Stopping background process

	i) Enter stop 'pid'.
	ii) On viewing jobs, the status of the given process can be seen as stopped.
	
5. Continuing Stopped background process

	i) Enter start 'pid'
	ii) ON viewing jobs change in status can be seen.
	
6. Stopping foreground process

	i) Press Ctrl+z
	
	Example:
	command: sleep 100
	This command will run for 100 seconds but if ctrl+z is pressed the shell will be ready for new process.
	
7. The program itself is immune to ctrl+z,ctrl+\and ctrl+c.

8. To exit the program type quit.

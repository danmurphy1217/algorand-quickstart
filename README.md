# algorand-quickstart
This shellscript provides you with the easiest ways to get up and running with an Algorand Node on macOS. It extrapolates some of the complexities of setting file permissions, downloading the correct scripts, and running these scripts and allows you to run just one command (`./quickstart/sh`) and start working with the Algorand APIs.

Here is a step-by-step breakdown of what this script does:

1. `cd` into the `$HOME` directory and create a new directory called **`node`**.
2. Create an environment variable called `ALGORAND_DATA` that tells the `update.sh` and `goal` scripts where to store important information pertaining to your node (API token, network URL and port, etc...)
3. Install the `update.sh` script and run it to install the node (or update it if it already exists). Additionally, we modify the permissions to make sure the file is executable.
4. Start the node with `./goal node start`
5. Check the status of the node with `./goal node status`

Here is how to run a node for yourself:
1. Run `curl https://raw.githubusercontent.com/danmurphy1217/algorand-quickstart/master/quick-algo -O`
2. Run `chmod +x quick-algo`
3. Run `./quick-algo`, fill in the questions with your preferred settings, and wait for the installation to finish. Then you're good to check out the **node** directory and play around with the `goal` API.

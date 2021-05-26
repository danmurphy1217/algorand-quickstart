#! /bin/bash

main () {
    echo "Executing script..."
    run
    echo "Finished."
}

run () {

    read -p 'Directory to Install Node [any valid directory : $HOME]: ' installation_loc # variable of where to install algorand node
    read -p 'Algorand Node Type [mainnet/testnet : mainnet]: ' node_type # type of algorand node to run
    read -p 'Run Fast Catchup? Read about Fast Catchup Here (https://developer.algorand.org/docs/run-a-node/setup/install/#sync-node-network-using-fast-catchup) [yes/no : no]: ' fast_catchup # run fast catchup during installation
    read -p 'Automate Fast Catchups [yes/no : yes]: ' automate_catchups # automate catchups?


    if [ -z "$installation_loc" ]; then # if $installation_loc is empty or not set
        installation_loc=$HOME
    fi;
    
    if [ -z "$node_type" ]; then # if $node_type is empty or not set
        node_type='mainnet'
    fi;

    if [ -z "$fast_catchup" ]; then # if $fast_catchup is empty or not set
        fast_catchup='no'
    fi;

    if [ -z "$automate_catchups" ]; then # if $automate_catchups is empty or not set
        automate_catchups='yes'
    fi;


    echo "Installing in:" $installation_loc
    change_dir $installation_loc # change to $HOME dir.
    FOLDER=$installation_loc/node

    if [ -d "$FOLDER" ]; then
        # if the node dir already exists, do not mkdir. Just cd into it.
        cd $installation_loc/node
    else
        # if the node dir does not exist, mkdir and then cd into it.
        mk_node_dir
    fi

    # create ALGORAND_DATA env var in installation dir
    create_env_var $installation_loc

    # install the update.sh script
    install https://raw.githubusercontent.com/algorand/go-algorand-doc/master/downloads/installers/update.sh

    # change the permissions of the update.sh file
    make_executable $installation_loc/node/update.sh
    # run the update.sh script
    run_update_script
    # start the node

    run_node $node_type

    # check the status of the node
    check_status

    if [[ $fast_catchup == "yes" ]]; then
        # do fast catchup
        
        # quietly download last catchpoint, extract the plaintext from the page w/ sed
        checkpoint=$(wget -qO- https://algorand-catchpoints.s3.us-east-2.amazonaws.com/channel/$node_type/latest.catchpoint | sed -e 's/<[^>]*>//g')

        cd $installation_loc/node
        ./goal node catchup $checkpoint -d $ALGORAND_DATA

        echo "Running fast catchup... This may take a few minutes."
    fi;

    if [[ $automate_catchups == "yes" ]]; then
        crontab -l >> $installation_loc/node/catchup_cron_job
        echo "00 * * * * $installation_loc/node/update.sh -i -c stable -d $ALGORAND_DATA > $installation_loc/node/sync.log 2>&1" >> $installation_loc/node/catchup_cron_job
        crontab $installation_loc/node/catchup_cron_job
    fi;
}

change_dir () {
    # change directory to $1
    # $1 must be a valid directory that is a subfolder of $(pwd)
    cd $1
}

mk_node_dir() {
    # make the node directory and cd into it

    mkdir node && cd node
    return 1
}

create_env_var() {
    # create ALGORAND_DATA env var
    # This tells the Algorand Node where to store data related to our node.

    if [ "$node_type" == 'mainnet' ]; then
        export ALGORAND_DATA=$1/node/data
    elif [ "$node_type" == 'testnet' ]; then
        export ALGORAND_DATA=$1/node/testnetdata
    fi;
    echo "Algorand Data Directory:" $ALGORAND_DATA
    return 1
}

install() {
    # Install $1, where $1 is a valid link to a downloadable file/folder.

    curl $1 -O
}

make_executable() {
    # make the passed file ($1) executable

    chmod 544 $1
}

run_update_script() {
    # updates the node to the latest version
    # -n ensures the node does not automatically start

    echo "RUNNING UPDATE.SH:"
    sh -c "yes | $installation_loc/node/update.sh -i -c stable -p $installation_loc/node -d $ALGORAND_DATA -n"
    echo ''
}

run_node () {
    cd $installation_loc/node
    echo "STARTING NODE:"
    if [ "$1" == "mainnet" ]; then
        # begins running the Algorand node
        ./goal node start -d $ALGORAND_DATA
        echo ''
    elif [ "$1" == "testnet" ]; then
        if [ -d "testnetdata" ]; then
            cp genesisfiles/testnet/genesis.json $installation_loc/node/testnetdata
            ./goal node start -d testnetdata
        else 
            mkdir testnetdata
            cp genesisfiles/testnet/genesis.json $installation_loc/node/testnetdata
            ./goal node start -d testnetdata
        fi;
    else
        echo "$1 is an invalid or currently unsupported Algorand Node Type. Try: mainnet, testnet"
        exit 1;
    fi;
}

check_status () {
    # Check the status of the Algorand Node.
    # If running, will return node information.
    # If error, will return "Cannot contact Algorand node..."

    echo "CHECKING NODE STATUS:"
    cd $installation_loc/node
    ./goal node status -d $ALGORAND_DATA
    echo ''
}

main
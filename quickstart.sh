#! /bin/bash

main () {
    echo "Executing script..."
    run
    echo "Finished!"
}

run () {
    
    cd $HOME
    FOLDER=$(pwd)/node

    if [ -d "$FOLDER" ]; then
        echo "node directory already exists, skipping..."
        cd node
    else
        mk_dir
    fi

    create_env_var # create ALGORAND_DATA env var
    install https://raw.githubusercontent.com/algorand/go-algorand-doc/master/downloads/installers/update.sh

    change_mode
    run_update
    run_node
    check_status
}

mk_dir() {
    mkdir node && cd node
    return 1
}

create_env_var() {
    export ALGORAND_DATA=$(pwd)/data
    echo $ALGORAND_DATA
    return 1
}

install() {
    curl $1 -O
}

change_mode() {
    chmod +x update.sh
}

run_update() {
    # updates the node to the latest version
    # -n ensures the node does not automatically start
    echo "RUNNING UPDATER:"
    sh -c "yes | ./update.sh -i -c stable -p ~/node -d ~/node/data -n"
}

run_node () {
    # begins running the Algorand node
    echo "STARTING NODE:"
    ./goal node start -d $ALGORAND_DATA
}

check_status () {
    # Check the status of the Algorand Node.
    # If running, will return node information.
    # If error, will return "Cannot contact Algorand node..."
    echo "CHECKING NODE STATUS:"
    ./goal node status -d $ALGORAND_DATA
}
main
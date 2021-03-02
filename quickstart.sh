#! /bin/bash

main () {
    echo "Executing script..."
    run
    echo "Finished."
}

run () {

    change_dir $HOME # change to $HOME dir.
    FOLDER=$(pwd)/node

    if [ -d "$FOLDER" ]; then
        # if the node dir already exists, do not mkdir. Just cd into it.
        echo "node directory already exists, skipping..."
        cd node
    else
        # if the node dir does not exist, mkdir and then cd into it.
        mk_node_dir
    fi

    # create ALGORAND_DATA env var
    create_env_var
    # install the update.sh script
    install https://raw.githubusercontent.com/algorand/go-algorand-doc/master/downloads/installers/update.sh

    # change the permissions of the update.sh file
    make_executable update.sh
    # run the update.sh script
    run_update_script
    # start the node
    run_node
    # check the status of the node
    check_status
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

    export ALGORAND_DATA=$(pwd)/data
    echo $ALGORAND_DATA
    return 1
}

install() {
    # Install $1, where $1 is a valid link to a downloadable file/folder.

    curl $1 -O
}

make_executable() {
    # make the passed file ($1) executable

    chmod +x $1
}

run_update_script() {
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
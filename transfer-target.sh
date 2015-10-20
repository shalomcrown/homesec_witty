#! /bin/bash


function rcopy()
# from, to_file_path
{
    cmd="scp $1 $2"
#    echo "  cmd: $cmd"
    $cmd
}

function rcmd()
# _cmd
{
    cmd="ssh $target $1"
#    echo "  cmd: $cmd"
    $cmd
}


executable=$1
executable="${executable%\"}" # quoted with \"
executable="${executable#\"}"
STRIP=strip

stripped=${executable}.stripped
echo "Strip executable $executable (to $stripped)"
$STRIP -v $executable -o $stripped

remote=${executable}.remote
echo "Transfer stripped executable $stripped (as $remote)"
echo -n " Test if stripped is transferred already..."
# Could check for unmodified files... (with md5sum)
# or use alternative method of transfer like rsync.
# simple (and unsafe) approach for now - make a local copy when the
# remote copy succeeded.
if cmp -s $stripped $remote; then
    echo " YES (IMHO)"
#    echo -n "wake up sshd..."
#    rcmd "echo done"
else
    echo " NO - start transfer"
    while ! ( rcopy "$stripped" "${remote##/*/}" && cp $stripped $remote ); do
	rcmd "killall gdbserver"
	rcmd "killall -9 $remote" # likely to be straced...
    done
fi


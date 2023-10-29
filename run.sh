if [[ ! -z "$1" ]] ; then
  if [[ "$1" == "-debug" ]] ; then
    ./compile.sh -debug && ./build/bangerking
  else
    ./compile.sh -file && ./build/bangerking $@
  fi
else
  ./compile.sh && ./build/bangerking
fi

bin=$1
if [ -z $1 ]
  then
    bin="GameEngineExample"
fi

./build.sh
if [ $? -eq 0 ];
then
  (cd ./bin/bin && clear && ./$bin ${@:2})
fi
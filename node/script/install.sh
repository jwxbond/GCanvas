echo "start  build @gcanvs/node"
sudo npm install cmake-js -g
mkdir -p ~/.gAssets/fonts/ &&  cp ./third_party/font/wqy-microhei.ttc ~/.gAssets/fonts/wqy-microhei.ttc &&  cp ./third_party/font/.fontcache  ~/.gAssets/fonts/.fontcache
npm run compile
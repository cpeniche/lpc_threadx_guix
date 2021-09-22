#!/bin/bash

threadx_main_dir=$HOME/projects/Azure/threadx
src_tree=(common/{inc,src} common_modules/{inc,module_lib/src,module_manager/{src,inc}} ports/cortex_m3/gnu/{src,inc})
project_dir=$1
for path in ${src_tree[@]}
do  
  if [ -z "$(ls -A $project_dir/$path)" ]; then    
    ln -s $threadx_main_dir/${path}/* $project_dir/${path}/
  fi
done

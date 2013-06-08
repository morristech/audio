# Copyright 2013, Qualcomm Innovation Center, Inc.
#
#    All rights reserved.
#    This file is licensed under the 3-clause BSD license in the NOTICE.txt
#    file for this project. A copy of the 3-clause BSD license is found at:
#
#        http://opensource.org/licenses/BSD-3-Clause. 
#
#    Unless required by applicable law or agreed to in writing, software
#    distributed under the license is distributed on an "AS IS" BASIS,
#    WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
#    See the license for the specific language governing permissions and
#    limitations under the license.

import os
import subprocess
import sys
import time

Import('env')

vars = Variables()
vars.Add(PathVariable('ABOUT_BASE', 'Base directory of About service', os.environ.get('ABOUT_BASE')))
vars.Add(PathVariable('ALAC_BASE', 'Base directory of ALAC', os.environ.get('ALAC_BASE')))
vars.Update(env)

Help(vars.GenerateHelpText(env))

# Dependent Projects
if not env.has_key('_ALLJOYNCORE_'):
    env.SConscript('../../alljoyn_core/SConscript')

sys.path.append('../../build_core/tools/scons')

if(not(env.has_key('BULLSEYE_BIN'))):
    print('BULLSEYE_BIN not specified')
else:
    env.PrependENVPath('PATH', env.get('BULLSEYE_BIN'))

# The return value is the collection of files installed in the build destination.
returnValue = []

# Make audio dist a sub-directory of the alljoyn dist.  This avoids any conflicts with alljoyn dist targets.
env['AUDIO_DISTDIR'] = env['DISTDIR'] + '/audio'
env['AUDIO_TESTDIR'] = env['TESTDIR'] + '/audio'

# Add support for multiple build targets in the same workset
env.VariantDir('$OBJDIR', 'src', duplicate = 0)
env.VariantDir('$OBJDIR/samples', 'samples', duplicate = 0)
env.VariantDir('$OBJDIR/test', 'test', duplicate = 0)

# Header file includes
env.Append(CPPPATH = [env.Dir('inc')])

# Audio streaming libraries
libs = env.SConscript('$OBJDIR/SConscript')
alljoyn_audio = env.Install('$AUDIO_DISTDIR/lib', libs)
returnValue += alljoyn_audio

env.Append(LIBPATH = [env.Dir('$AUDIO_DISTDIR/lib')])

# Set the alljoyn_audio library 
env.Prepend(LIBS = alljoyn_audio)

# Sample programs
returnValue += env.SConscript('$OBJDIR/samples/SConscript')

# Unit tests
env.SConscript('unit_test/SConscript', variant_dir='$OBJDIR/unittest', duplicate=0)

# Build docs
installDocs = env.SConscript('docs/SConscript')
env.Depends(installDocs, env.Glob('*.h'));
returnValue += installDocs

Return('returnValue')

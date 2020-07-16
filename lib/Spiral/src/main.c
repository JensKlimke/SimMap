/* ===================================================
 *  file:       main.c
 * ---------------------------------------------------
 *  purpose:	sample using the free method for
 *              spiral computation
 * ---------------------------------------------------
 *  using methods of CEPHES library
 * ---------------------------------------------------
 *  first edit:	09.03.2010 by M. Dupuis @ VIRES GmbH
 *  last mod.:  09.03.2010 by M. Dupuis @ VIRES GmbH
 * ===================================================
    Copyright 2010 VIRES Simulationstechnologie GmbH

    Licensed under the Apache License, Version 2.0 (the "License");
    you may not use this file except in compliance with the License.
    You may obtain a copy of the License at

        http://www.apache.org/licenses/LICENSE-2.0

    Unless required by applicable law or agreed to in writing, software
    distributed under the License is distributed on an "AS IS" BASIS,
    WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
    See the License for the specific language governing permissions and
    limitations under the License.
 */
 
#include <stdio.h>
#include "odrSpiral.h"

int main( int argc, char** argv )
{
    double s;
    double t;
    double x;
    double y;
    
    fprintf( stderr, "#  OpenDRIVE spiral example\n" );
    fprintf( stderr, "#  ------------------------\n#\n" );
    fprintf( stderr, "#  Computing a spiral with\n" );
    fprintf( stderr, "#    initial curvature =   0.000 1/m\n" );
    fprintf( stderr, "#    length            = 300.000 m\n" );
    fprintf( stderr, "#    curvDot           =   0.001 1/m2\n#\n" );
    
    for ( s = 0.0; s < 300.0; s += 1.0 )
    {
        odrSpiral( s, 0.001, &x, &y, &t );
        fprintf( stderr, "%10.4f %10.4f \n", x, y );
    }
    fprintf( stderr, "#  done.\n" );
    
}

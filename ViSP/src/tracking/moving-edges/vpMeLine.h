/****************************************************************************
 *
 * $Id: vpMeLine.h,v 1.13 2008-12-15 15:11:27 nmelchio Exp $
 *
 * Copyright (C) 1998-2006 Inria. All rights reserved.
 *
 * This software was developed at:
 * IRISA/INRIA Rennes
 * Projet Lagadic
 * Campus Universitaire de Beaulieu
 * 35042 Rennes Cedex
 * http://www.irisa.fr/lagadic
 *
 * This file is part of the ViSP toolkit.
 *
 * This file may be distributed under the terms of the Q Public License
 * as defined by Trolltech AS of Norway and appearing in the file
 * LICENSE included in the packaging of this file.
 *
 * Licensees holding valid ViSP Professional Edition licenses may
 * use this file in accordance with the ViSP Commercial License
 * Agreement provided with the Software.
 *
 * This file is provided AS IS with NO WARRANTY OF ANY KIND, INCLUDING THE
 * WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
 *
 * Contact visp@irisa.fr if any conditions of this licensing are
 * not clear to you.
 *
 * Description:
 * Moving edges.
 *
 * Authors:
 * Eric Marchand
 *
 *****************************************************************************/

/*!
  \file vpMeLine.h
  \brief Moving edges on a line
*/

#ifndef vpMeLine_HH
#define vpMeLine_HH


#include <math.h>
#include <iostream>


#include <visp/vpConfig.h>
#include <visp/vpMath.h>
#include <visp/vpMatrix.h>
#include <visp/vpMeTracker.h>


/*!
  \class vpMeLine

  \ingroup TrackingImageME

  \brief Class that tracks in an image a line moving edges.

  In this class the line is defined by its equation in the \f$ (i,j) =
  (line,column) \f$ image plane. Two kinds of parametrization are available to
  describe a 2D line. The first one corresponds to the following
  equation

  \f[ ai + bj + c = 0 \f]

  where \f$ i \f$ and \f$ j \f$ are the coordinates of the points
  belonging to the line. The line features are \f$ (a, b, c) \f$.

  The second way to write the line equation is to consider polar coordinates
  \f[ i \; cos(\theta) + j \; sin(\theta) - \rho = 0 \f]

  where \f$ i \f$ and \f$ j \f$ are still the coordinates of the
  points belonging to the line. But now the line features are \f$
  (\rho, \theta) \f$. The computation of \f$ \rho \f$ and \f$ \theta
  \f$ is easy thanks to \f$ (a, b, c) \f$.

  \f[ \theta = arctan(b/a) \f]
  \f[ \rho = -c/\sqrt{a^2+b^2} \f]

  The value of \f$ \theta \f$ is between \f$ 0 \f$ and \f$ 2\pi
  \f$. And the value of \f$ \rho \f$ can be positive or negative. The
  conventions to find the right values of the two features are
  illustrated in the following pictures.

  \image html vpMeLine.gif
  \image latex vpMeLine.ps  width=10cm

  The angle \f$\theta\f$ is computed thanks to the direction of the
  arrow. The arrow points to the side of the line which is darker.

  The code below shows how to use this class.
\code
#include <visp/vpConfig.h>
#include <visp/vpImage.h>
#include <visp/vpMeLine.h>
#include <visp/vpImagePoint.h>

int main()
{
  vpImage<unsigned char> I(240, 320);

  // Fill the image with a black rectangle
  I = 0;
  for (int i = 100; i < 180; i ++) {
    for (int j = 120; j < 250; j ++) {
      I[i][j] = 255;
    }
  }
    
  // Set the moving-edges tracker parameters
  vpMe me;
  me.setRange(25);
  me.setPointsToTrack(20);
  me.setThreshold(15000);
  me.setSampleStep(10);

  // Initialize the moving-edges line tracker parameters
  vpMeLine line;
  line.setMe(&me);

  // Initialize the location of the vertical line to track
  vpImagePoint ip1, ip2; // Two points belonging to the line to track 
  ip1.set_i( 120 );
  ip1.set_j( 119 );
  ip2.set_i( 170 );
  ip2.set_j( 122 );

  line.initTracking(I, ip1, ip2);

  while ( 1 )
  {
    // ... Here the code to read or grab the next image.

    // Track the line.
    line.track(I);
  }
  return 0;
}
\endcode

  \note It is possible to display the line as an overlay. For that you 
  must use the display function of the class vpMeLine.
*/

class VISP_EXPORT vpMeLine : public vpMeTracker
{
protected:
  vpMeSite PExt[2] ;

  double rho, theta ;
  double delta ,delta_1;
  double angle, angle_1;
  int sign;

public:
  double a; //!< Parameter a of the line equation a*i + b*j + c = 0
  double b; //!< Parameter a of the line equation a*i + b*j + c = 0
  double c; //!< Parameter a of the line equation a*i + b*j + c = 0

  vpMeLine() ;
  virtual ~vpMeLine() ;

  void display(vpImage<unsigned char>& I, vpColor col) ;

  void track(vpImage<unsigned char>& Im);

  void sample(vpImage<unsigned char>&image);
  void reSample(vpImage<unsigned char> &I) ;
  void leastSquare() ;
  void updateDelta();
  void setExtremities() ;
  void seekExtremities(vpImage<unsigned char> &I) ;
  void suppressPoints() ;

  void initTracking(vpImage<unsigned char> &I) ;
  void initTracking(vpImage<unsigned char> &I,
		    const vpImagePoint &ip1,
		    const vpImagePoint &ip2) ;

  void computeRhoTheta(vpImage<unsigned char> &I) ;
  double getRho() const ;
  double getTheta() const ;
  void getExtremities(vpImagePoint &ip1, vpImagePoint &ip2) ;

  static bool intersection(const vpMeLine &line1, const vpMeLine &line2, 
			   vpImagePoint &ip); 

  /****************************************************************

           Deprecated functions

  *****************************************************************/

#ifdef VISP_BUILD_DEPRECATED_FUNCTIONS
  /*!
    @name Deprecated functions
  */
  vp_deprecated void initTracking(vpImage<unsigned char> &I,
		    unsigned i1,unsigned j1,
		    unsigned i2, unsigned j2) ;
  vp_deprecated void getExtremities(double &i1, double &j1, double &i2, double &j2) ;
  vp_deprecated static bool intersection(const vpMeLine &line1, const vpMeLine &line2, 
			   double &i, double &j); 

#endif

};




#endif



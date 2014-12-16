#include "common_object.h"


#if (defined LB_FLUID_FORCING || defined LB_TEMPERATURE_FORCING || defined LB_SCALAR_FORCING)
void build_forcing(){
  int i, j, k , ii;
  my_double fnx,fny,fnz,kn;
  my_double x,y,z;
  my_double LX,LY,LZ,nu;
  my_double temp, fac, val; 
  vector dist,vel;
  vector u0, u0_all;
  my_double norm;
  my_double t0,t0_all;
  my_double s0,s0_all;

   LX = (my_double)(property.SX);
   LY = (my_double)(property.SY);
   LZ = (my_double)(property.SZ);

   nu = property.nu;

#ifdef LB_FLUID_FORCING_HIT  /* useful for HOMOGENEOUS ISOTROPIC TURBULENCE */
 #ifdef LB_FLUID_FORCING_HIT_RANDOM
   /* the phases are random */
      if(ROOT){ 
	for (ii=0; ii<nk; ii++){
	  phi[ii].x = myrand();
	  phi[ii].y = myrand();
	  phi[ii].z = myrand();
	}
      }
 #else
   /* the phases make a random walk */
      fac = sqrt(1.0/(my_double)randomization_itime);
      if(ROOT){ 
	for (ii=0; ii<nk; ii++){
	  val=(2.0*(myrand()-0.5) > 0.0)?1.0:-1.0;
	  phi[ii].x += val*fac;
	  val=(2.0*(myrand()-0.5) > 0.0)?1.0:-1.0;
	  phi[ii].y += val*fac;
	  val=(2.0*(myrand()-0.5) > 0.0)?1.0:-1.0;
	  phi[ii].z += val*fac;
	}
      }
 #endif
    /* the phases are broadcasted */
   MPI_Bcast(phi, nk, MPI_vector_type, 0, MPI_COMM_WORLD);

 #ifdef LB_FLUID_FORCING_HIT_ZEROMODE 
    /* compute the zero mode intensity (the mean velocity) */
    u0.x = u0_all.x = 0.0;
    u0.y = u0_all.y = 0.0;
    u0.z = u0_all.z = 0.0;

    for(k=BRD;k<LNZ+BRD;k++)
      for(j=BRD;j<LNY+BRD;j++)
	for(i=BRD;i<LNX+BRD;i++){ 
	    u0.x += u[IDX(i,j,k)].x;
            u0.y += u[IDX(i,j,k)].y;
            u0.z += u[IDX(i,j,k)].z;
	  }

    MPI_Allreduce(&u0, &u0_all, 1, MPI_vector_type, MPI_SUM_vector, MPI_COMM_WORLD );

    norm = sqrt(u0_all.x*u0_all.x + u0_all.y*u0_all.y + u0_all.z*u0_all.z);
    if(norm !=0.0){
      u0_all.x /= norm;
      u0_all.y /= norm;
      u0_all.z /= norm;
    }
 #endif
#endif

#ifdef LB_TEMPERATURE_FORCING_HIT
 #ifdef LB_TEMPERATURE_FORCING_HIT_RANDOM
   /* the phases are random */
      if(ROOT){ 
	for (ii=0; ii<nk; ii++){
	  phi_t[ii].x = myrand();
	  phi_t[ii].y = myrand();
	  phi_t[ii].z = myrand();
	}
      }
 #else
      /* the phases do random walk */
      fac = sqrt(1.0/(my_double)randomization_itime_t);
      if(ROOT){ 
	for (ii=0; ii<nk_t; ii++){
	  val=(2.0*(myrand()-0.5) > 0.0)?1.0:-1.0;
          phi_t[ii].x += val*fac;
          val=(2.0*(myrand()-0.5) > 0.0)?1.0:-1.0;
          phi_t[ii].y += val*fac;
          val=(2.0*(myrand()-0.5) > 0.0)?1.0:-1.0;
          phi_t[ii].z += val*fac;
	}
      }
#endif
      /* phases are boradcasted */
    MPI_Bcast(phi_t, nk_t, MPI_vector_type, 0, MPI_COMM_WORLD);

 #ifdef LB_TEMPERATURE_FORCING_HIT_ZEROMODE 
    /* compute the zero mode intensity (the mean temperature) */
    t0 = t0_all = 0.0;

    for(k=BRD;k<LNZ+BRD;k++)
      for(j=BRD;j<LNY+BRD;j++)
	for(i=BRD;i<LNX+BRD;i++){ 
	    t0 += t[IDX(i,j,k)];
	  }

    MPI_Allreduce(&t0, &t0_all, 1, MPI_DOUBLE, MPI_SUM, MPI_COMM_WORLD );

   norm = 1.0/(my_double)(property.SX*property.SY*property.SZ);
    if(norm !=0.0){
     t0_all *= norm;
    }
     t0_all=(t0_all > 0.0)?1.0:-1.0;
    //    if(ROOT)fprintf(stderr,"t0_all = %e\n",t0_all);
 #endif
#endif

#ifdef LB_SCALAR_FORCING_HIT
  #ifdef LB_SCALAR_FORCING_HIT_RANDOM
   /* the phases are random */
      if(ROOT){ 
	for (ii=0; ii<nk; ii++){
	  phi_s[ii].x = myrand();
	  phi_s[ii].y = myrand();
	  phi_s[ii].z = myrand();
	}
      }
 #else
      /* the phases do random walk */
      fac = sqrt(1.0/(my_double)randomization_itime_s);
      if(ROOT){ 
	for (ii=0; ii<nk_s; ii++){
	  val=(2.0*(myrand()-0.5) > 0.0)?1.0:-1.0;
          phi_s[ii].x += val*fac;
          val=(2.0*(myrand()-0.5) > 0.0)?1.0:-1.0;
          phi_s[ii].y += val*fac;
          val=(2.0*(myrand()-0.5) > 0.0)?1.0:-1.0;
          phi_s[ii].z += val*fac;
	}
      }
#endif
      /* phases are boradcasted */
    MPI_Bcast(phi_s, nk_s, MPI_vector_type, 0, MPI_COMM_WORLD);

 #ifdef LB_SCALAR_FORCING_HIT_ZEROMODE 
    /* compute the zero mode intensity (the mean temperature) */
    s0 = s0_all = 0.0;

    for(k=BRD;k<LNZ+BRD;k++)
      for(j=BRD;j<LNY+BRD;j++)
	for(i=BRD;i<LNX+BRD;i++){ 
	    s0 += s[IDX(i,j,k)];
	  }

    MPI_Allreduce(&s0, &s0_all, 1, MPI_DOUBLE, MPI_SUM, MPI_COMM_WORLD );

   norm = 1.0/(my_double)(property.SX*property.SY*property.SZ);
    if(norm !=0.0){
     s0_all *= norm;
    }
     s0_all=(s0_all > 0.0)?1.0:-1.0;
    //    if(ROOT)fprintf(stderr,"s0_all = %e\n",s0_all);
 #endif
#endif


 for(k=BRD;k<LNZ+BRD;k++)
    for(j=BRD;j<LNY+BRD;j++)
      for(i=BRD;i<LNX+BRD;i++){ 

   x = (my_double)center_V[IDX(i,j,k)].x;
   y = (my_double)center_V[IDX(i,j,k)].y;
   z = (my_double)center_V[IDX(i,j,k)].z;

#ifdef LB_FLUID_FORCING
	force[IDX(i,j,k)].x = 0.0;
	force[IDX(i,j,k)].y = 0.0;
	force[IDX(i,j,k)].z = 0.0;

#ifdef LB_FLUID_FORCING_POISEUILLE 
	/* note that the LX,LY,LZ dependence (i.e. the non-homogeneous direction) is here just an arbitrary choice */
	/* Here Amp indicates the maximal velocity at the center of the channel , note that in poiseuille flow U_max = Force * L^2/(8 \nu)  */
	force[IDX(i,j,k)].x += 2.0*property.Amp_x*((4.*nu)*pow(LY,-2.0));  
	force[IDX(i,j,k)].y += 2.0*property.Amp_y*((4.*nu)*pow(LX,-2.0));  
	force[IDX(i,j,k)].z += 2.0*property.Amp_z*((4.*nu)*pow(LY,-2.0));  
#endif

#ifdef LB_FLUID_FORCING_CHANNEL
	/* note that the LX,LY,LZ dependence (i.e. the non-homogeneous direction) is here just an arbitrary choice */
	/* Here Amp indicates the maximal velocity at the center of the turbulent channel , note that in this case we write U_max = sqrt(Force*L/2)  */
	force[IDX(i,j,k)].x += 2.0*pow(property.Amp_x,2.0)/LY;
	force[IDX(i,j,k)].y += 2.0*pow(property.Amp_y,2.0)/LX;
	force[IDX(i,j,k)].z += 2.0*pow(property.Amp_z,2.0)/LY;
#endif

#ifdef LB_FLUID_FORCING_CONSTANT_POWER
	/* Here Amp indicates the power Force*velocity  */
	/* note that out_all.uxt is computed only when we dumpe the averages */	
	if(out_all.ux != 0) force[IDX(i,j,k)].x += property.Amp_x/out_all.ux; 
	if(out_all.uy != 0) force[IDX(i,j,k)].y += property.Amp_y/out_all.uy;
	if(out_all.uz != 0) force[IDX(i,j,k)].z += property.Amp_z/out_all.uz;
#endif

#ifdef LB_FLUID_FORCING_KOLMOGOROV 
 
    kn=1.0;
    fnx=nu*pow(two_pi/LX,2.0);
    fny=nu*pow(two_pi/LY,2.0);
    //y = (my_double)center_V[IDX(i,j,k)].y;
    //x = (my_double)center_V[IDX(i,j,k)].x;

	/* along x */  
        force[IDX(i,j,k)].x += property.Amp_x*fny*sin(kn*two_pi*y/LY); 
	force[IDX(i,j,k)].y += property.Amp_y*fnx*sin(kn*two_pi*x/LX); 
	force[IDX(i,j,k)].z += property.Amp_z*fny*sin(kn*two_pi*y/LY);  

	//   fprintf(stderr,"property.Amp_x %e property.Amp_y %e property.Amp_z %e\n",property.Amp_x, property.Amp_y,property.Amp_z);
	//  exit(1);
#endif  


#ifdef LB_FLUID_FORCING_CELLULAR
 
    kn=0.5;
    //y = (my_double)center_V[IDX(i,j,k)].y;
    //x = (my_double)center_V[IDX(i,j,k)].x;

	/* along x */  
        force[IDX(i,j,k)].x += property.Amp_x*sin(kn*two_pi*x/LX)*cos(kn*two_pi*y/LY); 
	force[IDX(i,j,k)].y -= property.Amp_x*cos(kn*two_pi*x/LX)*sin(kn*two_pi*y/LY); 
	force[IDX(i,j,k)].z += 0.0; 

#endif  

#ifdef LB_FLUID_FORCING_HIT  /* for HOMOGENEOUS ISOTROPIC TURBULENCE */     
 #ifdef LB_FLUID_FORCING_HIT_LINEAR
   /* As in Phares L. Carroll and G. Blanquart PHYSICS OF FLUIDS 25, 105114 (2013) */
       fac = 0.5*((out_all.ux2 - out_all.ux*out_all.ux) + (out_all.uy2 - out_all.uy*out_all.uy) + (out_all.uz2 - out_all.uz*out_all.uz));	
       if(fac != 0.0) fac = 1.0/fac; else fac = 1.0;
      force[IDX(i,j,k)].x += fac*property.Amp_x*u[IDX(i,j,k)].x;
      force[IDX(i,j,k)].y += fac*property.Amp_y*u[IDX(i,j,k)].y;
      force[IDX(i,j,k)].z += fac*property.Amp_z*u[IDX(i,j,k)].z;
 #else
 #ifdef LB_FLUID_FORCING_HIT_ZEROMODE 
      /* the zero mode */
      //fac = sqrt(out_all.ux*out_all.ux + out_all.uy*out_all.uy + out_all.uz*out_all.uz);
      //if(fac != 0.0) fac = 1./fac; else fac = 1.0;
      force[IDX(i,j,k)].x += property.Amp_x*(- u0_all.x);
      force[IDX(i,j,k)].y += property.Amp_y*(- u0_all.y);
      force[IDX(i,j,k)].z += property.Amp_z*(- u0_all.z);
 #endif
      /* the other modes */
    for(ii=0; ii<nk; ii++){
      fac = pow(vk2[ii],-5./6.);
      force[IDX(i,j,k)].x += fac*property.Amp_x* ( sin(two_pi*(vk[ii].y*y/LY + phi[ii].y)) + sin(two_pi*(vk[ii].z*z/LZ + phi[ii].z)) );
      force[IDX(i,j,k)].y += fac*property.Amp_y* ( sin(two_pi*(vk[ii].x*x/LX + phi[ii].x)) + sin(two_pi*(vk[ii].z*z/LZ + phi[ii].z)) );
      force[IDX(i,j,k)].z += fac*property.Amp_z* ( sin(two_pi*(vk[ii].y*y/LY + phi[ii].y)) + sin(two_pi*(vk[ii].x*x/LX + phi[ii].x)) );
    }
 #endif  
#endif

#ifdef LB_FLUID_FORCING_ABSORB  /* attempt to implement an absorbing layer */
    fac = 0.8;
    dist.x = (x/LX - fac)/(1.0-fac);
    dist.y = (y/LY - fac)/(1.0-fac);
    dist.z = (z/LZ - fac)/(1.0-fac);

    if(dist.y>0 ){

    vel.x = 0.0; //out_all.ux;
    vel.y = out_all.uy;
    vel.z = 0.0; //out_all.uz;

    dist.x = 0.5 + pow(dist.x,2.0);
    //    dist.y = 1.0 / ( 0.5 + pow(dist.y,2.0) );
    //dist.y = 1.0;
    dist.z = 0.5 + pow(dist.z,2.0);  

    //fprintf(stderr,"%d %d %d %d %e\n",me, i,j,k, u[IDX(i,j,k)].y);
        
    //force[IDX(i,j,k)].x += -dist.y*(vel.x - u[IDX(i,j,k)].x);  
    //force[IDX(i,j,k)].y += -u[IDX(i,j,k)].y;  // dist.y*(vel.y  -u[IDX(i,j,k)].y);
    //force[IDX(i,j,k)].z += -dist.y*(vel.z  -u[IDX(i,j,k)].z);
    
     }
#endif

#ifdef LB_TEMPERATURE_BUOYANCY
	//my_double temp, fac;
#ifdef LB_TEMPERATURE_BUOYANCY_TREF
  temp = (t[IDX(i,j,k)] - property.T_top);
  //temp = (t[IDX(i,j,k)] - property.T_ref);
#else
  /* the good one for RB */  
  temp =  t[IDX(i,j,k)] - 0.5*(property.T_bot + property.T_top);
#endif

  //temp =  t[IDX(i,j,k)] - (-(property.deltaT/property.SY)*center_V[IDX(i,j,k)].y + property.T_bot) ;
  fac = property.beta_t*temp; 
  if(property.beta2_t != 0.0) fac += property.beta2_t*temp*temp;
  //fac = property.beta_t;

      force[IDX(i,j,k)].x += fac*property.gravity_x;
      force[IDX(i,j,k)].y += fac*property.gravity_y;
      force[IDX(i,j,k)].z += fac*property.gravity_z;

      //fprintf(stderr, "fy %e\n",property.gravity_y);
#endif

#ifdef LB_SCALAR_BUOYANCY
      //my_double temp, fac;
#ifdef LB_SCALAR_BUOYANCY_SREF
      temp = (s[IDX(i,j,k)] - property.S_top);
#else
      temp =  s[IDX(i,j,k)] - 0.5*(property.S_bot + property.S_top);
#endif

      fac = property.beta_s*temp;

      force[IDX(i,j,k)].x += fac*property.gravity_x;
      force[IDX(i,j,k)].y += fac*property.gravity_y;
      force[IDX(i,j,k)].z += fac*property.gravity_z;

#endif



#ifdef  LB_FLUID_FORCING_PENALIZATION
      my_double mask;
      /* penalization of a cube */
      /*  
      if( fabs(center_V[IDX(i,j,k)].x-property.SX/2.0) < 10 &&
	  fabs(center_V[IDX(i,j,k)].y) < 10 && 
	  fabs(center_V[IDX(i,j,k)].z-property.SZ/2.0) < 10  ) 
	mask=1.0; 
      else 
	mask=0.0;

      if( mask == 1.0 ){
	force[IDX(i,j,k)].x = -u[IDX(i,j,k)].x;  
	force[IDX(i,j,k)].y = -u[IDX(i,j,k)].y;
	force[IDX(i,j,k)].z = -u[IDX(i,j,k)].z;
	  }
      */

#ifdef LB_FLUID_FORCING_LANDSCAPE
       if(landscape[IDX(i, j, k)]>0.0){
#else
      /* small central spot penalization */
      mask = pow(center_V[IDX(i,j,k)].x-property.SX/2.0, 2.0)+pow(center_V[IDX(i,j,k)].y-property.SY/2.0, 2.0);
      if( mask < 10.0 ){       
#endif
	force[IDX(i,j,k)].x = -u[IDX(i,j,k)].x;  
	force[IDX(i,j,k)].y = -u[IDX(i,j,k)].y;
	force[IDX(i,j,k)].z = -u[IDX(i,j,k)].z;	
	  }
      
#endif


#endif

/* From here HERE SOURCE TERM ON TEMPERATURE FIELD */
#ifdef LB_TEMPERATURE_FORCING
      /* set to zero */ 
      t_source[IDX(i,j,k)] = 0.0;

   /* here we can for instance impose a temperature profile , or add a thermal source or make the field reactive*/
 
#ifdef LB_TEMPERATURE_FORCING_SOURCE
  /* mimic source term  */
      my_double spot;
      /* penalization of a cube */
      /*        
      if( fabs(center_V[IDX(i,j,k)].x-property.SX/2.0) < 10 &&
	  fabs(center_V[IDX(i,j,k)].y) < 10 && 
	  fabs(center_V[IDX(i,j,k)].z-property.SZ/2.0) < 10  ) 
	spot=1.0; 
      else 
	spot=0.0;

      if( spot == 1.0 ) t_source[IDX(i,j,k)] = -(t[IDX(i,j,k)] - property.T_bot);
      */
      /* small central spot penalization */
      spot = pow(center_V[IDX(i,j,k)].x-property.SX/2.0, 2.0)+pow(center_V[IDX(i,j,k)].y-property.SY/2.0, 2.0);
      if( spot < 1.0 ) t_source[IDX(i,j,k)] = -(t[IDX(i,j,k)] - property.T_bot);
#endif


#ifdef LB_TEMPERATURE_FORCING_ABSORB  /* attempt to implement an absorbing layer for temperature */
    fac = 0.5;
    dist.x = (x/LX - fac)/(1.0-fac);
    dist.y = (y/LY - fac)/(1.0-fac);
    dist.z = (z/LZ - fac)/(1.0-fac);

    if(dist.y>0){

    //dist.x = 0.5 + pow(dist.x,2.0);
    // dist.y = 1.0 / ( 0.5 + pow(dist.y,2.0) );
    //dist.z = 0.5 + pow(dist.z,2.0);  
        
     t_source[IDX(i,j,k)] = -(t[IDX(i,j,k)] - property.T_top);
       }
#endif


#ifdef LB_TEMPERATURE_FORCING_PROFILE
  /* impose a mean linear temperature profile , note that bc for temp shall be set to 0 */
  t_source[IDX(i,j,k)] = (property.deltaT/property.SY)*u[IDX(i,j,k)].y;
#endif

#ifdef LB_TEMPERATURE_FORCING_REACTION
  /* make the field reactive */
  t_source[IDX(i,j,k)] = property.Amp_t*t[IDX(i,j,k)]*(property.T_bot-t[IDX(i,j,k)]);
#endif

#ifdef LB_TEMPERATURE_FORCING_MONOD
  #ifdef LB_SCALAR
  /* make the field react to the scalar concentration */
  t_source[IDX(i,j,k)] = property.Amp_t * s[IDX(i,j,k)]/(0.5 + s[IDX(i,j,k)]) * t[IDX(i,j,k)];
  #endif
#endif

#ifdef LB_TEMPERATURE_FORCING_BULK
  t_source[IDX(i,j,k)] = property.Amp_t;
#ifdef LB_TEMPERATURE_FORCING_BULK_VARIABLE
  if (center_V[IDX(i,j,k)].x > property.SX/2.) t_source[IDX(i,j,k)] = 0.0;
#endif
#endif

#ifdef LB_TEMPERATURE_FORCING_RADIATION 
  //my_double coeff_exct = 0.5/property.SY;
  t_source[IDX(i,j,k)] = property.Amp_t*property.attenuation*exp(-property.attenuation*center_V[IDX(i,j,k)].y); 
#endif 

#ifdef LB_TEMPERATURE_FORCING_HIT /* for HOMOGENEOUS ISOTROPIC TURBULENCE on TEMPERATURE */     
 #ifdef LB_TEMPERATURE_FORCING_HIT_LINEAR
   /* Inspired by Phares L. Carroll and G. Blanquart PHYSICS OF FLUIDS 25, 105114 (2013) */	
      if(out_all.t2 != 0.0) fac = 1.0/out_all.t2; else fac = 1.0;
      t_source[IDX(i,j,k)] += fac*property.Amp_t*t[IDX(i,j,k)];
 #else
 #ifdef LB_TEMPERATURE_FORCING_HIT_ZEROMODE 
      /* the zero mode */
      t_source[IDX(i,j,k)] += property.Amp_t*(-t0_all);
 #endif
      /*the other modes */
    for(ii=0; ii<nk_t; ii++){
      fac = pow(vk2_t[ii],-5./6.);
      t_source[IDX(i,j,k)] += fac*property.Amp_t*( sin(two_pi*(vk_t[ii].x*x/LX + phi_t[ii].x)) + sin(two_pi*(vk_t[ii].y*y/LY + phi_t[ii].y)) + sin(two_pi*(vk_t[ii].z*z/LZ + phi_t[ii].z)) );
    }
 #endif
#endif

#ifdef LB_TEMPERATURE_FORCING_GRAD /* force temperature with constant gradient (along y) */
    t_source[IDX(i,j,k)] += -property.Amp_t*u[IDX(i,j,k)].y;
#endif

#endif


/* From here HERE SOURCE TERM ON SCALAR FIELD */
#ifdef LB_SCALAR_FORCING
      /* set to zero */ 
      s_source[IDX(i,j,k)] = 0.0;

 #ifdef LB_SCALAR_FORCING_REACTION
  /* make the field reactive */
  s_source[IDX(i,j,k)] = property.Amp_s*s[IDX(i,j,k)]*(property.S_bot-s[IDX(i,j,k)]);
 #endif

#ifdef LB_SCALAR_FORCING_MONOD
  #ifdef LB_TEMPERATURE
  /* make the field react to negatively to the t (phythoplancton) concentration */
  t_source[IDX(i,j,k)] = -property.Amp_s * s[IDX(i,j,k)]/(0.5 + s[IDX(i,j,k)]) * t[IDX(i,j,k)];
  #endif
#endif


#ifdef LB_SCALAR_FORCING_HIT /* for HOMOGENEOUS ISOTROPIC TURBULENCE on SCALAR */     
 #ifdef LB_SCALAR_FORCING_HIT_LINEAR
   /* Inspired by Phares L. Carroll and G. Blanquart PHYSICS OF FLUIDS 25, 105114 (2013) */	
      if(out_all.s2 != 0.0) fac = 1.0/out_all.s2; else fac = 1.0;
      s_source[IDX(i,j,k)] += fac*property.Amp_s*s[IDX(i,j,k)];
 #else
 #ifdef LB_SCALAR_FORCING_HIT_ZEROMODE 
      /* the zero mode */
      s_source[IDX(i,j,k)] += property.Amp_s*(-s0_all);
 #endif
      /*the other modes */
    for(ii=0; ii<nk_s; ii++){
      fac = pow(vk2_s[ii],-5./6.);
      s_source[IDX(i,j,k)] += fac*property.Amp_s*( sin(two_pi*(vk_s[ii].x*x/LX + phi_s[ii].x)) + sin(two_pi*(vk_s[ii].y*y/LY + phi_s[ii].y)) + sin(two_pi*(vk_s[ii].z*z/LZ + phi_s[ii].z)) );
    }
 #endif
#endif

#ifdef LB_SCALAR_FORCING_GRAD /* force scalar with constant gradient (along y) */
    s_source[IDX(i,j,k)] += -property.Amp_s*u[IDX(i,j,k)].y;
#endif

#endif


      }/* i,j,k */
}
#endif

#if (defined LB_FLUID_FORCING || defined LB_TEMPERATURE_FORCING || defined LB_SCALAR_FORCING)
void add_forcing(){
  int i, j, k, pp;
  my_double invtau,invtau_t,invtau_s;
  pop f_eq;
  my_double fac , fac_t, fac_s;
  vector d;
  my_double ux,uy,uz,cu,u2;
  vector vel;
  my_double rho ,temp , wgt2;
  pop p_eq , g_eq , h_eq;
  my_double mask;

#ifdef LB_FLUID
invtau = 1.0/property.tau_u;

 fac = 1.0; /* this is for streaming if GUO is not defined */
#ifdef METHOD_FORCING_GUO
  fac = (1.0-0.5*property.time_dt*invtau);
#endif
#ifdef METHOD_REDEFINED_POP
  fac = 1.0;
#ifdef METHOD_REDEFINED_POP_GUO
  fac = (1.0-0.5*property.time_dt*invtau);
#endif
#endif
#endif /* LB_FLUID */

#ifdef LB_TEMPERATURE
invtau_t = 1.0/property.tau_t;

#ifdef METHOD_FORCING_GUO
  fac_t = (1.0-0.5*property.time_dt*invtau_t);
#endif
#ifdef METHOD_REDEFINED_POP
  fac_t = 1.0;
#ifdef METHOD_REDEFINED_POP_GUO
  fac_t = (1.0-0.5*property.time_dt*invtau_t);
#endif
#endif
#endif /* LB_TEMPERATURE */


#ifdef LB_SCALAR
invtau_s = 1.0/property.tau_s;

#ifdef METHOD_FORCING_GUO
  fac_s = (1.0-0.5*property.time_dt*invtau_s);
#endif
#ifdef METHOD_REDEFINED_POP
  fac_s = 1.0;
#ifdef METHOD_REDEFINED_POP_GUO
  fac_s = (1.0-0.5*property.time_dt*invtau_s);
#endif
#endif
#endif /* LB_SCALAR */


  for(k=BRD;k<LNZ+BRD;k++)
    for(j=BRD;j<LNY+BRD;j++)
      for(i=BRD;i<LNX+BRD;i++){ 

	/* Here I prepare equilibrium pop for the direct forcing */
#ifdef  LB_FLUID_FORCING_DIRECT
      /* small central spot with velocity u=0 */
      mask = pow(center_V[IDX(i,j,k)].x-property.SX/2.0, 2.0)+pow(center_V[IDX(i,j,k)].y-property.SY/2.0, 2.0);
      vel.x = 0.0;
      vel.y = 0.0;
      vel.z = 0.0;
      rho = 1.0;
      p_eq = equilibrium_given_velocity(vel,rho);
#endif      

#ifdef  LB_TEMPERATURE_FORCING_DIRECT
      /* small central spot with velocity u=0 */
      mask = pow(center_V[IDX(i,j,k)].x-property.SX/2.0, 2.0)+pow(center_V[IDX(i,j,k)].y-property.SY/2.0, 2.0);
      vel.x = u[IDX(i,j,k)].x;
      vel.y = u[IDX(i,j,k)].y;
      vel.z = u[IDX(i,j,k)].z;
      temp = property.T_bot;
      g_eq = equilibrium_given_velocity(vel,temp);
#endif

      /* start loop on populations */
	for (pp=0; pp<NPOP; pp++){
	/* forcing */

#ifdef LB_FLUID_FORCING

#ifndef METHOD_FORCING_GUO  /* not defined METHOD_FORCING_GUO */
		  	  
	  /* Old version , simple but probably incomplete 
	    rho = dens[IDX(i,j,k)];	  
	    rhs_p[IDX(i,j,k)].p[pp] += invcs2*wgt[pp]*rho*force[IDX(i,j,k)].x*c[pp].x;
            rhs_p[IDX(i,j,k)].p[pp] += invcs2*wgt[pp]*rho*force[IDX(i,j,k)].y*c[pp].y;
            rhs_p[IDX(i,j,k)].p[pp] += invcs2*wgt[pp]*rho*force[IDX(i,j,k)].z*c[pp].z;
	  */

	  /* New version , like in GUO or in PhD thesis EPFL MALASPINAS */	  
	  
	rho = dens[IDX(i,j,k)];
	ux=u[IDX(i,j,k)].x;
	uy=u[IDX(i,j,k)].y;
	uz=u[IDX(i,j,k)].z;
        cu = (c[pp].x*ux + c[pp].y*uy + c[pp].z*uz);
        d.x = (c[pp].x-ux)*invcs2 + c[pp].x*cu*invcs4;
        d.y = (c[pp].y-uy)*invcs2 + c[pp].y*cu*invcs4;
        d.z = (c[pp].z-uz)*invcs2 + c[pp].z*cu*invcs4;

       rhs_p[IDX(i,j,k)].p[pp] += fac*wgt[pp]*rho*force[IDX(i,j,k)].x*d.x;
       rhs_p[IDX(i,j,k)].p[pp] += fac*wgt[pp]*rho*force[IDX(i,j,k)].y*d.y;
       rhs_p[IDX(i,j,k)].p[pp] += fac*wgt[pp]*rho*force[IDX(i,j,k)].z*d.z;   

	  
#ifdef METHOD_LOG
	    fac = 3.0*wgt[pp]*property.tau_u*exp(-p[IDX(i,j,k)].p[pp]*invtau);
	    rhs_p[IDX(i,j,k)].p[pp] += fac*force[IDX(i,j,k)].x*c[pp].x;
            rhs_p[IDX(i,j,k)].p[pp] += fac*force[IDX(i,j,k)].y*c[pp].y;
            rhs_p[IDX(i,j,k)].p[pp] += fac*force[IDX(i,j,k)].z*c[pp].z;
#endif
	    
#else   
	    /* This is METHOD_FORCING_GUO */
	rho = dens[IDX(i,j,k)];    
	ux=u[IDX(i,j,k)].x;
	uy=u[IDX(i,j,k)].y;
	uz=u[IDX(i,j,k)].z;
        cu = (c[pp].x*ux + c[pp].y*uy + c[pp].z*uz);
        d.x = (c[pp].x-ux)*invcs2 + c[pp].x*cu*invcs4;
        d.y = (c[pp].y-uy)*invcs2 + c[pp].y*cu*invcs4;
        d.z = (c[pp].z-uz)*invcs2 + c[pp].z*cu*invcs4;

       rhs_p[IDX(i,j,k)].p[pp] += fac*wgt[pp]*rho*force[IDX(i,j,k)].x*d.x;
       rhs_p[IDX(i,j,k)].p[pp] += fac*wgt[pp]*rho*force[IDX(i,j,k)].y*d.y;
       rhs_p[IDX(i,j,k)].p[pp] += fac*wgt[pp]*rho*force[IDX(i,j,k)].z*d.z;       
#endif

#ifdef  LB_FLUID_FORCING_DIRECT

#ifdef LB_FLUID_FORCING_LANDSCAPE
       if(landscape[IDX(i, j, k)]>0.0){
#else
      if( sqrt(mask) < 10.0 ){
#endif
	/* this implementation works only when METHOD_EULER is used */
	rhs_p[IDX(i,j,k)].p[pp] =  (p_eq.p[pp] - p[IDX(i,j,k)].p[pp] )/property.time_dt;
	  }      
#endif

#endif

#ifdef LB_TEMPERATURE_FORCING
#ifndef METHOD_FORCING_GUO
       /* Not Guo here */
	rhs_g[IDX(i,j,k)].p[pp] += wgt[pp]*t_source[IDX(i,j,k)];
#else
      /* The forcing is as in Malaspinas PhD pag. 93 (bottom)*/	    
        ux = u[IDX(i, j, k)].x;
        uy = u[IDX(i, j, k)].y;
        uz = u[IDX(i, j, k)].z;                         
        u2 = (ux * ux + uy * uy + uz * uz);
        cu = (c[pp].x * ux + c[pp].y * uy + c[pp].z * uz);
	wgt2=(1.0 + invcs2 * cu + invtwocs4 * cu * cu - invtwocs2 * u2);
	rhs_g[IDX(i,j,k)].p[pp] += fac_t*wgt2*wgt[pp]*t_source[IDX(i,j,k)];
#endif


#ifdef  LB_TEMPERATURE_FORCING_DIRECT
      if( sqrt(mask) < 10.0 ){
	/* this implementation works only when METHOD_EULER is used */
	rhs_g[IDX(i,j,k)].p[pp] =  (g_eq.p[pp] - g[IDX(i,j,k)].p[pp] )/property.time_dt;
	  }      
#endif
#endif

#ifdef LB_SCALAR_FORCING
#ifndef METHOD_FORCING_GUO
       /* not Guo here  */
	    rhs_h[IDX(i,j,k)].p[pp] += wgt[pp]*s_source[IDX(i,j,k)];
#else
        ux = u[IDX(i, j, k)].x;
        uy = u[IDX(i, j, k)].y;
        uz = u[IDX(i, j, k)].z;                         
        u2 = (ux * ux + uy * uy + uz * uz);
        cu = (c[pp].x * ux + c[pp].y * uy + c[pp].z * uz);
	wgt2=(1.0 + invcs2 * cu + invtwocs4 * cu * cu - invtwocs2 * u2);
	rhs_h[IDX(i,j,k)].p[pp] += fac_s*wgt[pp]*wgt2*s_source[IDX(i,j,k)];
#endif
#endif


	}/* pp */
      }/* i,j,k */
}
#endif


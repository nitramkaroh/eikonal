/*
 *
 *                 #####    #####   ######  ######  ###   ###
 *               ##   ##  ##   ##  ##      ##      ## ### ##
 *              ##   ##  ##   ##  ####    ####    ##  #  ##
 *             ##   ##  ##   ##  ##      ##      ##     ##
 *            ##   ##  ##   ##  ##      ##      ##     ##
 *            #####    #####   ##      ######  ##     ##
 *
 *
 *             OOFEM : Object Oriented Finite Element Code
 *
 *               Copyright (C) 1993 - 2011   Borek Patzak
 *
 *
 *
 *       Czech Technical University, Faculty of Civil Engineering,
 *   Department of Structural Mechanics, 166 29 Prague, Czech Republic
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program; if not, write to the Free Software
 *  Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 */

#ifndef RankineMatGrad_h

#include "rankinemat.h"
#include "structuralnonlocalmaterialext.h"
#include "nonlocmatstiffinterface.h"
#include "cltypes.h"

#include "sparsemtrx.h"
#include "dynalist.h"

#ifdef __OOFEG
 #include "oofeggraphiccontext.h"
 #include "conTable.h"
#endif

namespace oofem {

/**
 * Gradient rankine material status.
 */
class RankineMatGradStatus : public RankineMatStatus
{
protected:
    double kappa_nl;
    double kappa_hat;

public:
    RankineMatGradStatus(int n, Domain *d, GaussPoint *g);
    ~RankineMatGradStatus() { }

    void printOutputAt(FILE *file, TimeStep *tStep);

    // definition
    const char *giveClassName() const { return "RankineMatGradStatus"; }
    classType giveClassID() const { return RankineMatClass; }

    virtual void initTempStatus();
    virtual void updateYourself(TimeStep *tStep);

    void setKappa_nl(double kap) { kappa_nl = kap; }
    void setKappa_hat(double kap) { kappa_hat = kap; }
    double giveKappa_nl() { return kappa_nl; }
    double giveKappa_hat() { return kappa_hat; }
};


/**
 * Gradient rankine material.
 */
class RankineMatGrad : public RankineMat
{
protected:
    double R;
    double mParam;
    double negligible_damage;

public:
    RankineMatGrad(int n, Domain *d);
    ~RankineMatGrad() {; }

    const char *giveClassName() const { return "RankineMatGrad"; }
    classType giveClassID() const { return RankineMatClass; }
    const char *giveInputRecordName() const { return "RankineMatGrad"; }

    IRResultType initializeFrom(InputRecord *ir);
    int hasMaterialModeCapability(MaterialMode mode);

    void giveCharacteristicMatrix(FloatMatrix &answer, MatResponseForm form, MatResponseMode rMode, GaussPoint *gp, TimeStep *tStep);
    virtual void givePlaneStressStiffMtrx(FloatMatrix & answer,  MatResponseForm, MatResponseMode, GaussPoint * gp,  TimeStep * tStep);
    void givePlaneStressKappaMatrix(FloatMatrix &answer, MatResponseForm form, MatResponseMode mode, GaussPoint *gp, TimeStep *tStep);
    void givePlaneStressGprime(FloatMatrix &answer, MatResponseForm form, MatResponseMode mode, GaussPoint *gp, TimeStep *tStep);
    void giveInternalLength(FloatMatrix &answer, MatResponseForm form, MatResponseMode mode, GaussPoint *gp, TimeStep *tStep);
    void giveRealStressVector(FloatArray &answer,  MatResponseForm form, GaussPoint *gp, const FloatArray &strainVector, TimeStep *tStep);

    void computeCumPlastStrain(double &kappa, GaussPoint *gp, TimeStep *tStep);
    double giveNonlocalCumPlasticStrain(GaussPoint *gp);
    void performPlasticityReturn(GaussPoint *gp, const FloatArray &totalStrain);

    LinearElasticMaterial *giveLinearElasticMaterial() { return linearElasticMaterial; }

    int giveIPValue(FloatArray &answer, GaussPoint *aGaussPoint, InternalStateType type, TimeStep *tStep);
    InternalStateValueType giveIPValueType(InternalStateType type);
    int giveIntVarCompFullIndx(IntArray &answer, InternalStateType type, MaterialMode mmode);
    int giveIPValueSize(InternalStateType type, GaussPoint *gp);

protected:
    MaterialStatus *CreateStatus(GaussPoint *gp) const { return new RankineMatGradStatus(1, RankineMat :: domain, gp); }
};
} // end namespace oofem
#define RankineMatGrad_h
#endif

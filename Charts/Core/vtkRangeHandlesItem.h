/*=========================================================================

  Program:   Visualization Toolkit
  Module:    vtkRangeHandlesItem.h

  Copyright (c) Ken Martin, Will Schroeder, Bill Lorensen
  All rights reserved.
  See Copyright.txt or http://www.kitware.com/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
     PURPOSE.  See the above copyright notice for more information.

=========================================================================*/

/**
 * @class   vtkRangeHandlesItem
 * @brief   item to show and control the range of a vtkColorTransferFunction
 *
 * vtkRangeHandlesItem provides range handles painting and management
 * for a provided vtkColorTransferFunction.
 * Handles can be moved by clicking on them.
 * The range is shown when hovering or moving the handles.
 * It emits a StartInteractionEvent when starting to interact with a handle,
 * an InteractionEvent when interacting with a handle and an EndInteractionEvent
 * when releasing a handle.
 * It emits a LeftMouseButtonDoubleClickEvent when double clicked.
 *
 * @sa
 * vtkControlPointsItem
 * vtkScalarsToColorsItem
 * vtkColorTransferFunctionItem
 */

#ifndef vtkRangeHandlesItem_h
#define vtkRangeHandlesItem_h

#include "vtkChartsCoreModule.h" // For export macro
#include "vtkPlotRangeHandlesItem.h"

class vtkColorTransferFunction;

class VTKCHARTSCORE_EXPORT vtkRangeHandlesItem : public vtkPlotRangeHandlesItem
{
public:
  vtkTypeMacro(vtkRangeHandlesItem, vtkPlotRangeHandlesItem);
  void PrintSelf(ostream& os, vtkIndent indent) override;
  static vtkRangeHandlesItem* New();

  /**
   * Overridden to check that a color transfer function has been set before
   * painting.
   */
  bool Paint(vtkContext2D* painter) override;

  /**
   * Overridden to get the bounds from the color transfer function range.
   */
  void GetBounds(double bounds[4]) override;

  /**
   * Overridden to return the range of the color transfer function.
   * Use this method by observing EndInteractionEvent
   */
  void GetHandlesRange(double range[2]) override;

  //@{
  /**
   * Get/set the color transfer function to interact with.
   */
  void SetColorTransferFunction(vtkColorTransferFunction* ctf);
  vtkGetObjectMacro(ColorTransferFunction, vtkColorTransferFunction);
  //@}

  /**
   * Compute the handles draw range by using the handle width and the transfer
   * function.
   */
  void ComputeHandlesDrawRange();

  //@{
  /**
   * Overridden to force using desynchronized vertical handles
   */
  void SynchronizeRangeHandlesOn() override;

  void SetSynchronizeHandlesRange(bool vtkNotUsed(synchronize))
  {
    this->Superclass::SynchronizeRangeHandlesOff();
  }

  void SetHandleOrientation(int vtkNotUsed(orientation))
  {
    this->Superclass::SetHandleOrientation(Orientation::VERTICAL);
  }
  //@}

protected:
  vtkRangeHandlesItem();
  ~vtkRangeHandlesItem() override;

  /**
   * Overridden to clamp the handle position in the color tranfer function
   * range.
   */
  void SetActiveHandlePosition(double position) override;

private:
  vtkRangeHandlesItem(const vtkRangeHandlesItem&) = delete;
  void operator=(const vtkRangeHandlesItem&) = delete;

  vtkColorTransferFunction* ColorTransferFunction = nullptr;
};

#endif // vtkRangeHandlesItem_h

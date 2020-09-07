/*=========================================================================

  Program:   Visualization Toolkit
  Module:    vtkPlotBarRangeHandlesItem.h

  Copyright (c) Ken Martin, Will Schroeder, Bill Lorensen
  All rights reserved.
  See Copyright.txt or http://www.kitware.com/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
     PURPOSE.  See the above copyright notice for more information.

=========================================================================*/

/**
 * @class   vtkPlotBarRangeHandlesItem
 * @brief   item to show and control the range of a vtkPlot
 *
 * vtkPlotBarRangeHandlesItem provides range handles painting and management
 * for a provided vtkPlot.
 * Handles can be moved by clicking on them.
 * The range is shown when hovering or moving the handles.
 * It emits a StartInteractionEvent when starting to interact with a handle,
 * an InteractionEvent when interacting with a handle and an EndInteractionEvent
 * when releasing a handle.
 *
 * @sa
 * vtkPlot
 */

#ifndef vtkPlotBarRangeHandlesItem_h
#define vtkPlotBarRangeHandlesItem_h

#include "vtkChartsCoreModule.h" // For export macro
#include "vtkPlotRangeHandlesItem.h"

class vtkPlotBar;

class VTKCHARTSCORE_EXPORT vtkPlotBarRangeHandlesItem : public vtkPlotRangeHandlesItem
{
public:
  vtkTypeMacro(vtkPlotBarRangeHandlesItem, vtkPlotRangeHandlesItem);
  static vtkPlotBarRangeHandlesItem* New();

  /**
   * Recover the bounds of the item
   */
  void GetBounds(double bounds[4]) override;

  //@{
  /**
   * Get/set the color transfer function to interact with.
   */
  void SetPlotBar(vtkPlotBar* ctf);
  vtkGetObjectMacro(PlotBar, vtkPlotBar);
  //@}

protected:
  vtkPlotBarRangeHandlesItem();
  ~vtkPlotBarRangeHandlesItem() override;

  
  /**
   * Internal method to set the ActiveHandlePosition
   * and compute the ActiveHandleRangeValue accordingly
   */
  void SetActiveHandlePosition(double position) override;

private:
  vtkPlotBarRangeHandlesItem(const vtkPlotBarRangeHandlesItem&) = delete;
  void operator=(const vtkPlotBarRangeHandlesItem&) = delete;

  vtkPlotBar* PlotBar = nullptr;
};

#endif // vtkPlotBarRangeHandlesItem_h

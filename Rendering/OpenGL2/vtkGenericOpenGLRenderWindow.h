/*=========================================================================

  Program:   Visualization Toolkit
  Module:    vtkRenderWindow.h

  Copyright (c) Ken Martin, Will Schroeder, Bill Lorensen
  All rights reserved.
  See Copyright.txt or http://www.kitware.com/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
     PURPOSE.  See the above copyright notice for more information.

=========================================================================*/
/**
 * @class   vtkGenericOpenGLRenderWindow
 * @brief   platform independent render window
 *
 *
 * vtkGenericOpenGLRenderWindow provides a skeleton for implementing a render window
 * using one's own OpenGL context and drawable.
 * To be effective, one must register an observer for WindowMakeCurrentEvent,
 * WindowIsCurrentEvent and WindowFrameEvent.  When this class sends a WindowIsCurrentEvent,
 * the call data is an bool* which one can use to return whether the context is current.
*/

#ifndef vtkGenericOpenGLRenderWindow_h
#define vtkGenericOpenGLRenderWindow_h

#include "vtkRenderingOpenGL2Module.h" // For export macro
#include "vtkOpenGLRenderWindow.h"

class VTKRENDERINGOPENGL2_EXPORT vtkGenericOpenGLRenderWindow :
  public vtkOpenGLRenderWindow
{
public:
  static vtkGenericOpenGLRenderWindow* New();
  vtkTypeMacro(vtkGenericOpenGLRenderWindow, vtkOpenGLRenderWindow);
  void PrintSelf(ostream& os, vtkIndent indent);
protected:
  vtkGenericOpenGLRenderWindow();
  ~vtkGenericOpenGLRenderWindow();

public:

  //! Cleans up graphics resources allocated in the context for this VTK scene.
  void Finalize();

  //! flush the pending drawing operations
  //! Class user may to watch for WindowFrameEvent and act on it
  void Frame();

  //! Makes the context current.  It is the class user's
  //! responsibility to watch for WindowMakeCurrentEvent and set it current.
  void MakeCurrent();

  //! Returns if the context is current.  It is the class user's
  //! responsibility to watch for WindowIsCurrentEvent and set the bool* flag
  //! passed through the call data parameter.
  bool IsCurrent();

  //! Returns if OpenGL is supported.  It is the class user's
  //! responsibility to watch for WindowSupportsOpenGLEvent and set the int* flag
  //! passed through the call data parameter.
  int SupportsOpenGL();

  //! Returns if the context is direct.  It is the class user's
  //! responsibility to watch for WindowIsDirectEvent and set the int* flag
  //! passed through the call data parameter.
  int IsDirect();

  // {@
  //! set the drawing buffers to use
  void SetFrontBuffer(unsigned int);
  void SetFrontLeftBuffer(unsigned int);
  void SetFrontRightBuffer(unsigned int);
  void SetBackBuffer(unsigned int);
  void SetBackLeftBuffer(unsigned int);
  void SetBackRightBuffer(unsigned int);
  // }@

  //! no-op (for API compat with OpenGL1).
  void PushState() {}
  //! no-op (for API compat with OpenGL1).
  void PopState() {}

  // {@
  //! does nothing
  void SetWindowId(void*);
  void* GetGenericWindowId();
  void SetDisplayId(void*);
  void SetParentId(void*);
  void* GetGenericDisplayId();
  void* GetGenericParentId();
  void* GetGenericContext();
  void* GetGenericDrawable();
  void SetWindowInfo(char*);
  void SetParentInfo(char*);
  int* GetScreenSize();
  void Start();
  void HideCursor();
  void ShowCursor();
  void SetFullScreen(int);
  void WindowRemap();
  int  GetEventPending();
  void SetNextWindowId(void*);
  void SetNextWindowInfo(char*);
  void CreateAWindow();
  void DestroyWindow();
  // }@

  //@{
  /**
   * Allow to update state within observer callback without changing
   * data argument and MTime.
   */
  void SetIsDirect(int newValue);
  void SetSupportsOpenGL(int newValue);
  void SetIsCurrent(bool newValue);
  //@}

  /**
   * Override the Render method to do some state management.
   * This method saves the OpenGL state before asking its child renderers to
   * render their image. Once this is done, the OpenGL state is restored.
   * \sa vtkOpenGLRenderWindow::SaveGLState()
   * \sa vtkOpenGLRenderWindow::RestoreGLState()
   */
  void Render();

  /**
   * Overridden to pass explicitly specified MaximumHardwareLineWidth, if any.
   */
  float GetMaximumHardwareLineWidth() VTK_OVERRIDE;

  //@{
  /**
   * Specificy a non-zero line width to force the hardware line width determined
   * by the window.
   */
  vtkSetClampMacro(ForceMaximumHardwareLineWidth, float, 0, VTK_FLOAT_MAX);
  vtkGetMacro(ForceMaximumHardwareLineWidth, float);
  //@}

  //@{
  /**
   * Set this to true to indicate that the context is now ready. For backwards
   * compatibility reasons, it's set to true by default. If set to false, the
   * `Render` call will be skipped entirely.
   */
  vtkSetMacro(ReadyForRendering, bool);
  vtkGetMacro(ReadyForRendering, bool);

protected:
  int DirectStatus;
  int SupportsOpenGLStatus;
  bool CurrentStatus;
  float ForceMaximumHardwareLineWidth;
  bool ReadyForRendering;

private:
  vtkGenericOpenGLRenderWindow(const vtkGenericOpenGLRenderWindow&) VTK_DELETE_FUNCTION;
  void operator=(const vtkGenericOpenGLRenderWindow&) VTK_DELETE_FUNCTION;
};

#endif

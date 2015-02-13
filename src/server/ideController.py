import cherrypy
from ws4py.websocket import WebSocket
from genshi.template import TemplateLoader
from threading import Lock


class IDEController(object):
  """
  Controller of the IDE/Editing part
  """

  def __init__(self, template_path, logger):
    """
    IDEController initialiser

    @param template_path: Path to the template directory
    @param logger: The CIDE.py logger instance
    """
    self._loader = TemplateLoader(template_path, auto_reload=True)

    self._logger = logger

    # XXX Temp dummy vars
    self.data = ""

    self._logger.debug("IDEController instance created")

  @cherrypy.expose
  def index(self):
    return "Hello world"
    # TODO Return page/template render for the IDE part
    # tmpl = loader.load('edit.html')
    # # set args in generate as key1=val1, key2=val2
    # stream = tmpl.generate()
    # return stream.render('html')

  @cherrypy.expose
  def sendEdit(self, content):
    # XXX Temp dummy method for test
    self.data += content
    for user in IDEWebSocket.IDEClients:
      user.send(self.data)

  @cherrypy.expose
  def refreshEdit(self):
    return self.data

  @cherrypy.expose
  def ws(self):
    """
    Method must exist to serve as a exposed hook for the websocket
    """
    self._logger.info("WS creation request from {0}:{1}".format(cherrypy.request.remote.ip,
                                                                cherrypy.request.remote.port))


class IDEWebSocket(WebSocket):
  IDEClients = set()
  __lock = Lock()

  def __init__(self, *args, **kw):
    WebSocket.__init__(self, *args, **kw)
    with self.__lock:
      self.IDEClients.add(self)

  def closed(self, code, reason=None):
    self.IDEClients.remove(self)


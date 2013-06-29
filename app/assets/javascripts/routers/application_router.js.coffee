#= require_self
#= require_tree .

$$.module 'Router', (self) ->
  self.on 'before:start', ->
    $$.router = new self.ApplicationRouter
    $$.routers = {}
  
  self.path = (router, action, options={}) ->
    if _.isString(action)
      router = $$.routers[router]
    else
      options = action || {}
      action = router
      router = $$.router
    URI('/'+ router._path(action, options))
  
  self.navigate = (router, action, options={}, navigateOptions={}) ->
    uri = self.path(router, action, options)
    navigateOptions = options unless _.isString(action)
    navigateOptions.trigger ||= true
    $$.router.navigate(uri.toString(), navigateOptions)
  
  class self.ApplicationRouter extends Backbone.Router
    routes: '': 'root'
    root: -> $$.routers.epgs.index()
    
    initialize: (options) ->
      super(options)
      @_setParams()
    
    _setParams: (params={}) ->
      @params ||= {}
      _.extend(@params, @_checkParams(params))
    
    _checkParams: (params={}) -> params
    
    _path: (action, params={}) ->
      params = @_checkParams(params)
      path = _(@routes).invert()[action] || ''
      path = path.replace(/:(\w+)/g,'{$1}')
      URI.expand(path, params)
